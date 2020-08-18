#include "BuildTimeline.h"

#include <cassert>
#include <CppBuildInsights.hpp>

#include "AnalysisData\Utilities\CppBuildInsightsDataConversion.h"

BuildTimeline::BuildTimeline()
    : m_entries()
    , m_roots()
    , m_remapper()
{
}

BuildTimeline::~BuildTimeline()
{
}

void BuildTimeline::AddRootEntry(const CppBI::Activities::Activity& activity)
{
    TimelineEntry* entry = AddEntry(activity);

    assert(std::find(m_roots.begin(), m_roots.end(), entry) == m_roots.end());
    m_roots.push_back(entry);
}

void BuildTimeline::AddNestedEntry(const CppBI::Activities::Activity& parent,
                                   const CppBI::Activities::Activity& activity)
{
    TimelineEntry* parentEntry = GetEntry(parent.EventInstanceId());
    assert(parentEntry != nullptr);

    TimelineEntry* entry = AddEntry(activity);
    parentEntry->AddChild(entry);
}

TimelineEntry* BuildTimeline::AddEntry(const CppBI::Activities::Activity& activity)
{
    TimelineEntry entry = TimelineEntry(activity.EventInstanceId(),
                                        activity.EventName(),
                                        Utilities::CppBuildInsightsDataConversion::Timestamp(activity.StartTimestamp(), activity.TickFrequency()),
                                        Utilities::CppBuildInsightsDataConversion::Timestamp(activity.StopTimestamp(), activity.TickFrequency()),
                                        activity.ProcessId(),
                                        activity.ThreadId(),
                                        activity.ProcessorIndex());

    auto result = m_entries.try_emplace(activity.EventInstanceId(), entry);
    assert(result.second);

    return &result.first->second;
}

void BuildTimeline::FinishEntry(const CppBI::Activities::Activity& activity)
{
    TimelineEntry* entry = GetEntry(activity.EventInstanceId());
    assert(entry != nullptr);

    entry->SetFinishTimestamp(Utilities::CppBuildInsightsDataConversion::Timestamp(activity.StopTimestamp(), activity.TickFrequency()));

    m_remapper.CalculateLocalChildrenData(entry);
}

void BuildTimeline::RemoveHierarchy(const TEventInstanceId& parentId, const TEventInstanceId& id)
{
    TimelineEntry* parentEntry = GetEntry(parentId);
    assert(parentEntry != nullptr);

    // ensure parent no longer points to it
    parentEntry->RemoveChild(id);

    // remove entry from the hierarchy (no need to ensure intermediate parents don't point to intermediate children, because the full hierarchy will be deleted)
    RemoveEntry(id);
}

void BuildTimeline::RemoveEntry(const TEventInstanceId& id)
{
    TimelineEntry* entry = GetEntry(id);
    if (entry != nullptr)
    {
        for (TimelineEntry* child : entry->GetChildren())
        {
            RemoveEntry(child->GetId());
        }

        m_entries.erase(id);
    }
}

void BuildTimeline::FinishTimeline()
{
    assert(m_remapper.GetRemappings().empty());

    m_remapper.Calculate(this);
}

void BuildTimeline::UpdateEntryName(const TEventInstanceId& id, const std::string& name)
{
    TimelineEntry* entry = GetEntry(id);
    assert(entry != nullptr);

    entry->SetName(name);
}

void BuildTimeline::AddEntryProperty(const TEventInstanceId& id, const std::string& key, const std::string& value)
{
    TimelineEntry* entry = GetEntry(id);
    assert(entry != nullptr);

    entry->AddProperty(key, value);
}

TimelineEntry* BuildTimeline::GetEntry(const TEventInstanceId& id)
{
    auto itEntry = m_entries.find(id);
    if (itEntry == m_entries.end())
    {
        return nullptr;
    }

    return &itEntry->second;
}
