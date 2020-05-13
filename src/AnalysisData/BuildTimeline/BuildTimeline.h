#pragma once

#include <unordered_map>
#include <vector>

#include "AnalysisData\BuildTimeline\ProcessThreadRemapping\PackedProcessThreadRemapper.h"
#include "AnalysisData\BuildTimeline\TimelineTypes.h"
#include "AnalysisData\BuildTimeline\TimelineEntry.h"

namespace Microsoft
{
    namespace Cpp
    {
        namespace BuildInsights
        {
            class TraceInfo;

            namespace Activities
            {
                class Activity;
            }
        }
    }
}
namespace CppBI = Microsoft::Cpp::BuildInsights;

class BuildTimeline
{
public:
    BuildTimeline();
    ~BuildTimeline();

    void AddRootEntry(const CppBI::Activities::Activity& activity);
    void AddNestedEntry(const CppBI::Activities::Activity& parent,
                        const CppBI::Activities::Activity& activity);
    void FinishEntry(const CppBI::Activities::Activity& activity);
    void FinishTimeline();

    void UpdateEntryName(const TEventInstanceId& id, const std::string& name);
    void AddEntryProperty(const TEventInstanceId& id, const std::string& key, const std::string& value);

    inline const std::vector<TimelineEntry*>& GetRoots() const { return m_roots; }
    inline const TProcessThreadRemappings& GetProcessThreadRemappings() const { return m_remapper.GetRemappings(); }

private:
    std::unordered_map<TEventInstanceId, TimelineEntry> m_entries;
    std::vector<TimelineEntry*> m_roots;
    PackedProcessThreadRemapper m_remapper;

    TimelineEntry* AddEntry(const CppBI::Activities::Activity& activity);
    TimelineEntry* GetEntry(const TEventInstanceId& id);
};
