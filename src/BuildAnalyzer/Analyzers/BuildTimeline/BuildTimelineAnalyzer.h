#pragma once

#include <CppBuildInsights.hpp>
namespace CppBI = Microsoft::Cpp::BuildInsights;

#include "AnalysisData\BuildTimeline\BuildTimeline.h"

class BuildTimelineAnalyzer : public CppBI::IAnalyzer
{
public:
	BuildTimelineAnalyzer();
	virtual ~BuildTimelineAnalyzer();

	CppBI::AnalysisControl OnStartActivity(const CppBI::EventStack& eventStack) override;
	CppBI::AnalysisControl OnStopActivity(const CppBI::EventStack& eventStack) override;

private:
	BuildTimeline m_buildTimeline;

	void OnActivityStartRoot(const CppBI::Activities::Activity& activity);
	void OnActivityStartNested(const CppBI::Activities::Activity& parent,
							   const CppBI::Activities::Activity& activity);
	void OnActivityFinished(const CppBI::Activities::Activity& activity);
};
