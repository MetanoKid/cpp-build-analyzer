#include "FilterTimelineAnalyzer.h"

FilterTimelineAnalyzer::FilterTimelineAnalyzer(const std::chrono::milliseconds& ignoreFunctionsUnder,
                                               const std::chrono::milliseconds& ignoreTemplatesUnder)
    : CppBI::IAnalyzer()
    , m_ignoredActivities()
    , m_ignoreFunctionsUnder(std::chrono::duration_cast<std::chrono::nanoseconds>(ignoreFunctionsUnder))
    , m_ignoreTemplatesUnder(std::chrono::duration_cast<std::chrono::nanoseconds>(ignoreTemplatesUnder))
{
}

FilterTimelineAnalyzer::~FilterTimelineAnalyzer()
{
}

CppBI::AnalysisControl FilterTimelineAnalyzer::OnStopActivity(const CppBI::EventStack& eventStack)
{
    bool processed =
        CppBI::MatchEventInMemberFunction(eventStack.Back(), this, &FilterTimelineAnalyzer::FilterFunction) ||
        CppBI::MatchEventInMemberFunction(eventStack.Back(), this, &FilterTimelineAnalyzer::FilterTemplateInstantiation);

    return CppBI::AnalysisControl::CONTINUE;
}

void FilterTimelineAnalyzer::FilterFunction(const CppBI::Activities::Function& function)
{
    if (function.Duration() < m_ignoreFunctionsUnder)
    {
        m_ignoredActivities.emplace(function.EventInstanceId());
    }
}

void FilterTimelineAnalyzer::FilterTemplateInstantiation(const CppBI::Activities::TemplateInstantiation& templateInstantiation)
{
    if (templateInstantiation.Duration() < m_ignoreTemplatesUnder)
    {
        m_ignoredActivities.emplace(templateInstantiation.EventInstanceId());
    }
}
