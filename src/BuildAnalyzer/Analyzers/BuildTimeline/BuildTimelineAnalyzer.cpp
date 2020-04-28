#include "BuildTimelineAnalyzer.h"

#include <cassert>

#include "AnalysisData\Utilities\CppBuildInsightsDataConversion.h"

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
	bool processedBaseData =
		CppBI::MatchEventStackInMemberFunction(eventStack,   this, &BuildTimelineAnalyzer::OnActivityStartNested) ||
		CppBI::MatchEventInMemberFunction(eventStack.Back(), this, &BuildTimelineAnalyzer::OnActivityStartRoot);
	assert(processedBaseData);

	bool processedSpecificData =
		CppBI::MatchEventInMemberFunction(eventStack.Back(), this, &BuildTimelineAnalyzer::OnFrontEndFile) ||
		CppBI::MatchEventInMemberFunction(eventStack.Back(), this, &BuildTimelineAnalyzer::OnFunction) ||
		CppBI::MatchEventInMemberFunction(eventStack.Back(), this, &BuildTimelineAnalyzer::OnTemplateInstantiation);

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

void BuildTimelineAnalyzer::OnFrontEndFile(const CppBI::Activities::FrontEndFile& frontEndFile)
{
	m_buildTimeline.UpdateEntryName(frontEndFile, frontEndFile.Path());
}

void BuildTimelineAnalyzer::OnFunction(const CppBI::Activities::Function& function)
{
	m_buildTimeline.UpdateEntryName(function, Utilities::CppBuildInsightsDataConversion::UndecorateFunction(function.Name()));
}

void BuildTimelineAnalyzer::OnTemplateInstantiation(const CppBI::Activities::TemplateInstantiation& templateInstantiation)
{
	// TODO: check docs and understand the format
}
