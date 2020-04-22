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

	inline const TTimeElapsedPerOcurrencePerConcept& GetData() const { return m_functionCompilationsData; }

private:
	TTimeElapsedPerOcurrencePerConcept m_functionCompilationsData;

	void OnFunctionCompiled(const CppBI::Activities::Function& function);
};
