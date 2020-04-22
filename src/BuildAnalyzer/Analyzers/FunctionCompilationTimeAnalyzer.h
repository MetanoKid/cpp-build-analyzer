#pragma once

#include <CppBuildInsights.hpp>
namespace CppBI = Microsoft::Cpp::BuildInsights;

#include "AnalysisData\TimeElapsedPerOccurrencePerConcept.h"

class FunctionCompilationTimeAnalyzer : public CppBI::IAnalyzer
{
public:
	FunctionCompilationTimeAnalyzer();
	virtual ~FunctionCompilationTimeAnalyzer();

	CppBI::AnalysisControl OnStopActivity(const CppBI::EventStack& eventStack) override;

	inline const TTimeElapsedPerOcurrencePerConcept& GetFunctionDurations() const { return m_functionDurations; }

private:
	TTimeElapsedPerOcurrencePerConcept m_functionDurations;

	void OnFunctionCompiled(const CppBI::Activities::Function& function);
};
