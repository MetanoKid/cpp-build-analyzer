#include "FilterTimelineAnalyzer.h"

FilterTimelineAnalyzer::FilterTimelineAnalyzer(const std::chrono::milliseconds& ignoreFunctionsUnder)
    : CppBI::IAnalyzer()
    , m_ignoredActivities()
    , m_ignoreFunctionsUnder(std::chrono::duration_cast<std::chrono::nanoseconds>(ignoreFunctionsUnder))
{
}

FilterTimelineAnalyzer::~FilterTimelineAnalyzer()
{
}

CppBI::AnalysisControl FilterTimelineAnalyzer::OnStopActivity(const CppBI::EventStack& eventStack)
{
    bool processed =
        CppBI::MatchEventInMemberFunction(eventStack.Back(), this, &FilterTimelineAnalyzer::FilterFunction);

    return CppBI::AnalysisControl::CONTINUE;
}

void FilterTimelineAnalyzer::FilterFunction(const CppBI::Activities::Function& function)
{
    if (function.Duration() < m_ignoreFunctionsUnder)
    {
        m_ignoredActivities.emplace(function.EventInstanceId());
    }
}
