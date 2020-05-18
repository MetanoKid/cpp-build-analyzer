#include "TimelinePackedProcessThreadRemappingAnalyzer.h"

TimelinePackedProcessThreadRemappingAnalyzer::TimelinePackedProcessThreadRemappingAnalyzer()
    : CppBI::IAnalyzer()
{
}

TimelinePackedProcessThreadRemappingAnalyzer::~TimelinePackedProcessThreadRemappingAnalyzer()
{
}

CppBI::AnalysisControl TimelinePackedProcessThreadRemappingAnalyzer::OnStopActivity(const CppBI::EventStack& eventStack)
{
    CppBI::MatchEventInMemberFunction(eventStack.Back(), this, &TimelinePackedProcessThreadRemappingAnalyzer::CalculateChildrenData);

    return CppBI::AnalysisControl::CONTINUE;
}

void TimelinePackedProcessThreadRemappingAnalyzer::CalculateChildrenData(const CppBI::Activities::Activity& activity)
{
}
