#pragma once

#include "AnalysisData\BuildTimeline\TimelineTypes.h"
#include "AnalysisData\BuildTimeline\ProcessThreadRemapping\ProcessThreadRemapping.h"

class BuildTimeline;
class TimelineEntry;

class PackedProcessThreadRemapper
{
public:
    PackedProcessThreadRemapper();
    ~PackedProcessThreadRemapper();

    void CalculateLocalChildrenData(const TimelineEntry* entry);
    void Calculate(const BuildTimeline* timeline);

    inline const TProcessThreadRemappings& GetRemappings() const { return m_remappings; }

private:
    struct LocalThreadData
    {
        // local offset, only taking parallel siblings into account
        TThreadId RawLocalThreadId = 0;

        // number of threads required to fit subhierarchy
        // (i.e. it can have parallel entries within parallel entries)
        TThreadId RequiredThreadIdToFitHierarchy = 0;

        // local ThreadId within its parent, with fitted hierarchy included
        TThreadId CalculatedLocalThreadId = 0;
    };

    std::unordered_map<TEventInstanceId, LocalThreadData> m_localThreadData;
    TProcessThreadRemappings m_remappings;

    void CalculateChildrenRawLocalThreadId(const TimelineEntry* entry);
    void CalculateChildrenRequiredThreadIdToFitHierarchy(const TimelineEntry* entry);

    void CalculateRootProcessIdRemappings(const BuildTimeline* timeline);
    void CalculateThreadIdRemappings(const BuildTimeline* timeline);
    void CalculateThreadIdRemap(const TimelineEntry* entry,
                                const TProcessId& parentProcessId,
                                const TThreadId& parentAbsoluteThreadId);
};
