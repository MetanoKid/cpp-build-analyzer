#include "BuildTimelineAnalyzer.h"

#include <cassert>

BuildTimelineAnalyzer::BuildTimelineAnalyzer()
	: CppBI::IAnalyzer()
	, m_buildTimeline()
{
}

BuildTimelineAnalyzer::~BuildTimelineAnalyzer()
{
}

CppBI::AnalysisControl BuildTimelineAnalyzer::OnStartActivity(const CppBI::EventStack& eventStack)
{
	bool processedBaseData = CppBI::MatchEventStackInMemberFunction(eventStack,   this, &BuildTimelineAnalyzer::OnActivityStartNested) ||
							 CppBI::MatchEventInMemberFunction(eventStack.Back(), this, &BuildTimelineAnalyzer::OnActivityStartRoot);
	assert(processedBaseData);

	return CppBI::AnalysisControl::CONTINUE;
}

CppBI::AnalysisControl BuildTimelineAnalyzer::OnStopActivity(const CppBI::EventStack& eventStack)
{
	bool processed = CppBI::MatchEventInMemberFunction(eventStack.Back(), this, &BuildTimelineAnalyzer::OnActivityFinished);
	assert(processed);

	return CppBI::AnalysisControl::CONTINUE;
}

void BuildTimelineAnalyzer::OnActivityStartRoot(const CppBI::Activities::Activity& activity)
{
	m_buildTimeline.AddRootEntry(activity);
}

void BuildTimelineAnalyzer::OnActivityStartNested(const CppBI::Activities::Activity& parent,
											   const CppBI::Activities::Activity& activity)
{
	m_buildTimeline.AddNestedEntry(parent, activity);
}

void BuildTimelineAnalyzer::OnActivityFinished(const CppBI::Activities::Activity& activity)
{
	m_buildTimeline.FinishEntry(activity);
}
