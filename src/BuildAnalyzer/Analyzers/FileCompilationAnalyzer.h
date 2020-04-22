#pragma once

#include <CppBuildInsights.hpp>
namespace CppBI = Microsoft::Cpp::BuildInsights;

class FileCompilationAnalyzer : public CppBI::IAnalyzer
{
public:
	FileCompilationAnalyzer();
	virtual ~FileCompilationAnalyzer();

	CppBI::AnalysisControl OnStopActivity(const CppBI::EventStack& eventStack) override;

private:
	void OnFrontEndPassCompleted(const CppBI::Activities::FrontEndPass& frontEndPass);
	void OnBackEndPassCompleted(const CppBI::Activities::BackEndPass& backEndPass);
};
