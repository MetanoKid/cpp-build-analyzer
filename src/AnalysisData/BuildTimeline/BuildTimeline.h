#pragma once

#include <unordered_map>
#include <vector>

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

    void UpdateEntryName(const TEventInstanceId& id, const std::string& name);

    inline const std::vector<TimelineEntry*>& GetRoots() const { return m_roots; }

private:
    std::unordered_map<TEventInstanceId, TimelineEntry> m_entries;
    std::vector<TimelineEntry*> m_roots;

    TimelineEntry* AddEntry(const CppBI::Activities::Activity& activity);
    TimelineEntry* GetEntry(const TEventInstanceId& id);
};
