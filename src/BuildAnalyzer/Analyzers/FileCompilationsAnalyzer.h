#pragma once

#include <CppBuildInsights.hpp>
namespace CppBI = Microsoft::Cpp::BuildInsights;

class FileCompilationsAnalyzer : public CppBI::IAnalyzer
{
public:
	FileCompilationsAnalyzer();
	virtual ~FileCompilationsAnalyzer();

	CppBI::AnalysisControl OnStopActivity(const CppBI::EventStack& eventStack) override;

private:
	void OnFrontEndPassCompleted(const CppBI::Activities::FrontEndPass& frontEndPass);
	void OnBackEndPassCompleted(const CppBI::Activities::BackEndPass& backEndPass);
};
