#include "PackedProcessThreadRemapper.h"

#include <cassert>

#include "AnalysisData\BuildTimeline\BuildTimeline.h"
#include "AnalysisData\BuildTimeline\TimelineEntry.h"

PackedProcessThreadRemapper::PackedProcessThreadRemapper()
    : m_localThreadData()
    , m_remappings()
{
}

PackedProcessThreadRemapper::~PackedProcessThreadRemapper()
{
}

void PackedProcessThreadRemapper::CalculateLocalChildrenData(const TimelineEntry* entry)
{
    assert(entry != nullptr);

    // leafs can add their data right away
    if (entry->GetChildren().size() == 0)
    {
        // because it's a leaf, we're sure there's no data for it yet
        assert(m_localThreadData.find(entry->GetId()) == m_localThreadData.end());

        LocalThreadData data;
        data.RequiredThreadIdToFitHierarchy = 0;

        m_localThreadData.try_emplace(entry->GetId(), data);
    }
    else
    {
        CalculateChildrenRawLocalThreadId(entry);
        CalculateChildrenRequiredThreadIdToFitHierarchy(entry);
    }
}

void PackedProcessThreadRemapper::CalculateChildrenRawLocalThreadId(const TimelineEntry* entry)
{
    assert(entry->GetChildren().size() > 0);

    const std::vector<TimelineEntry*>& children = entry->GetChildren();
    std::vector<TThreadId> overlappingLocalThreadId;
    for (auto itChild = children.begin(); itChild != children.end(); ++itChild)
    {
        const TimelineEntry* child = *itChild;
        overlappingLocalThreadId.clear();
        
        // children are sorted by start time, so we only have to check previous siblings for overlaps
        for (auto itPrecedingSibling = std::make_reverse_iterator(itChild); itPrecedingSibling != children.rend(); ++itPrecedingSibling)
        {
            const TimelineEntry* precedingSibling = *itPrecedingSibling;

            if (child->OverlapsWith(precedingSibling))
            {
                // preceding sibling has been processed already, so we're sure to have its data
                auto it = m_localThreadData.find(precedingSibling->GetId());
                assert(it != m_localThreadData.end());
                
                overlappingLocalThreadId.push_back(it->second.RawLocalThreadId);
            }
        }

        // calculate first ThreadId where we don't overlap with any sibling
        TThreadId localThreadId = 0;
        while (std::find(overlappingLocalThreadId.begin(), overlappingLocalThreadId.end(), localThreadId) != overlappingLocalThreadId.end())
        {
            ++localThreadId;
        }

        // we're sure there's data for us because child entries get processed before their parents
        auto it = m_localThreadData.find(child->GetId());
        assert(it != m_localThreadData.end());

        it->second.RawLocalThreadId = localThreadId;
    }
}

void PackedProcessThreadRemapper::CalculateChildrenRequiredThreadIdToFitHierarchy(const TimelineEntry* entry)
{
    assert(entry->GetChildren().size() > 0);

    // group children with their partial LocalThreadData
    typedef std::pair<const TimelineEntry*, LocalThreadData*> TEntryWithLocalData;
    std::vector<TEntryWithLocalData> sortedChildrenWithData;
    for (auto&& child : entry->GetChildren())
    {
        auto it = m_localThreadData.find(child->GetId());
        assert(it != m_localThreadData.end());

        sortedChildrenWithData.emplace_back(entry, &it->second);
    }

    // sort them by their raw local ThreadId as a way of "grouping" by local ThreadId
    assert(sortedChildrenWithData.size() == entry->GetChildren().size());
    std::sort(sortedChildrenWithData.begin(), sortedChildrenWithData.end(), [](const TEntryWithLocalData& lhs,
                                                                               const TEntryWithLocalData& rhs)
    {
        return lhs.second->RawLocalThreadId < rhs.second->RawLocalThreadId;
    });

    // our goal is to find how many extra ThreadId are required for each "group" to fit their hierarchies.
    // entries within the same "group" are considered "sequential" (i.e. they don't interact with each
    // other and we need to get the maximum number of ThreadId for their subhierarchies) while those
    // in separate "groups" are considered "parallel" (i.e. add previous required ThreadId to know the
    // real local ThreadId)
    TThreadId currentRawLocalThreadId = sortedChildrenWithData[0].second->RawLocalThreadId;
    TThreadId currentRequiredThreadIdToFitHierarchy = 0;
    TThreadId accumulatedRequiredThreadIdToFitHierarchy = 0;
    for (auto&& data : sortedChildrenWithData)
    {
        // "sequential" entries only account for the highest number of ThreadId to fit their hierarchies
        if (currentRawLocalThreadId == data.second->RawLocalThreadId)
        {
            if (data.second->RequiredThreadIdToFitHierarchy > currentRequiredThreadIdToFitHierarchy)
            {
                currentRequiredThreadIdToFitHierarchy = data.second->RequiredThreadIdToFitHierarchy;
            }
        }
        // when we reach a "parallel" entry, prepare data for next "group"
        else
        {
            accumulatedRequiredThreadIdToFitHierarchy += currentRequiredThreadIdToFitHierarchy;

            currentRawLocalThreadId = data.second->RawLocalThreadId;
            currentRequiredThreadIdToFitHierarchy = data.second->RequiredThreadIdToFitHierarchy;
        }

        // calculate the real ThreadId
        data.second->CalculatedLocalThreadId = data.second->RawLocalThreadId +
                                               accumulatedRequiredThreadIdToFitHierarchy;
    }

    // last "sequence" wasn't taken into account because we didn't iterate into a separate "group"
    accumulatedRequiredThreadIdToFitHierarchy += currentRequiredThreadIdToFitHierarchy;

    // we're sure we don't have data yet because we're a parent and parents get processed after their children
    assert(m_localThreadData.find(entry->GetId()) == m_localThreadData.end());

    LocalThreadData data;
    data.RequiredThreadIdToFitHierarchy = currentRawLocalThreadId + accumulatedRequiredThreadIdToFitHierarchy;

    m_localThreadData.try_emplace(entry->GetId(), data);
}

void PackedProcessThreadRemapper::Calculate(const BuildTimeline* timeline)
{
    assert(timeline != nullptr);
    assert(m_remappings.empty());

    CalculateRootProcessIdRemappings(timeline);
    CalculateThreadIdRemappings(timeline);
}

void PackedProcessThreadRemapper::CalculateRootProcessIdRemappings(const BuildTimeline* timeline)
{
    const std::vector<TimelineEntry*>& roots = timeline->GetRoots();
    std::vector<TProcessId> overlappingProcessId;
    for (auto itRoot = roots.begin(); itRoot != roots.end(); ++itRoot)
    {
        const TimelineEntry* root = *itRoot;
        overlappingProcessId.clear();

        // roots are sorted by start time, so we only have to check previous siblings for overlaps
        for (auto itPrecedingSibling = std::make_reverse_iterator(itRoot); itPrecedingSibling != roots.rend(); ++itPrecedingSibling)
        {
            const TimelineEntry* precedingSibling = *itPrecedingSibling;

            if (root->OverlapsWith(precedingSibling))
            {
                // preceding sibling has been processed already, so we're sure to have its data
                auto it = m_remappings.find(precedingSibling->GetId());
                assert(it != m_remappings.end());

                overlappingProcessId.push_back(it->second.ProcessId);
            }
        }

        // calculate first ProcessId where we don't overlap with any sibling
        TProcessId processId = 0;
        while (std::find(overlappingProcessId.begin(), overlappingProcessId.end(), processId) != overlappingProcessId.end())
        {
            ++processId;
        }

        // we're sure we don't have any data for us yet
        assert(m_remappings.find(root->GetId()) == m_remappings.end());

        ProcessThreadRemap remap;
        remap.ProcessId = processId;
        remap.ThreadId = 0; // roots always take the lowest ThreadId

        m_remappings.try_emplace(root->GetId(), remap);
    }
}

void PackedProcessThreadRemapper::CalculateThreadIdRemappings(const BuildTimeline* timeline)
{
    for (auto&& root : timeline->GetRoots())
    {
        auto it = m_remappings.find(root->GetId());
        assert(it != m_remappings.end());

        CalculateThreadIdRemap(root, it->second.ProcessId, it->second.ThreadId);
    }
}

void PackedProcessThreadRemapper::CalculateThreadIdRemap(const TimelineEntry* entry,
                                                         const TProcessId& parentProcessId,
                                                         const TThreadId& parentAbsoluteThreadId)
{
    for (auto&& child : entry->GetChildren())
    {
        // we're sure to have calculated local ThreadId for this child (calculated in CalculateLocalChildrenData)
        auto itLocalData = m_localThreadData.find(child->GetId());
        assert(itLocalData != m_localThreadData.end());

        // we're sure we don't have a remap yet
        assert(m_remappings.find(child->GetId()) == m_remappings.end());

        ProcessThreadRemap remap;
        remap.ProcessId = parentProcessId;
        remap.ThreadId = parentAbsoluteThreadId + itLocalData->second.CalculatedLocalThreadId;

        m_remappings.try_emplace(child->GetId(), remap);

        // let children calculate
        CalculateThreadIdRemap(child, remap.ProcessId, remap.ThreadId);
    }
}
