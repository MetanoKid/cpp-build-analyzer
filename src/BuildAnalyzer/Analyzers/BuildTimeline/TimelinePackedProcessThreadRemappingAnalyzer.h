#pragma once

#include <CppBuildInsights.hpp>
namespace CppBI = Microsoft::Cpp::BuildInsights;

class TimelinePackedProcessThreadRemappingAnalyzer : public CppBI::IAnalyzer
{
public:
    TimelinePackedProcessThreadRemappingAnalyzer();
    virtual ~TimelinePackedProcessThreadRemappingAnalyzer();

    CppBI::AnalysisControl OnStopActivity(const CppBI::EventStack& eventStack) override;

private:
    void CalculateChildrenData(const CppBI::Activities::Activity& activity);
};
