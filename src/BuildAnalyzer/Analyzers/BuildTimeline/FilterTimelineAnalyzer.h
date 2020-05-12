#pragma once

#include <unordered_set>
#include <chrono>

#include <CppBuildInsights.hpp>
namespace CppBI = Microsoft::Cpp::BuildInsights;

#include "AnalysisData\BuildTimeline\IgnoredEntries.h"

class FilterTimelineAnalyzer : public CppBI::IAnalyzer
{
public:
    FilterTimelineAnalyzer(const std::chrono::milliseconds& ignoreFunctionsUnder);
    virtual ~FilterTimelineAnalyzer();

    CppBI::AnalysisControl OnStopActivity(const CppBI::EventStack& eventStack) override;

    inline const TIgnoredEntries& GetIgnoredEntries() const { return m_ignoredActivities; }

private:
    TIgnoredEntries m_ignoredActivities;
    std::chrono::nanoseconds m_ignoreFunctionsUnder;

    void FilterFunction(const CppBI::Activities::Function& function);
};
