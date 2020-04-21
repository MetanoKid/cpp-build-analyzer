#pragma once

#include <CppBuildInsights.hpp>

namespace CppBI = Microsoft::Cpp::BuildInsights;

class SlowFunctionCompilationAnalyzer : public CppBI::IAnalyzer
{
public:
	SlowFunctionCompilationAnalyzer();
	virtual ~SlowFunctionCompilationAnalyzer();

	CppBI::AnalysisControl OnStopActivity(const CppBI::EventStack& eventStack) override;

private:
	void OnFunction(const CppBI::Activities::Function& function);
};
