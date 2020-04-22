#include "FunctionCompilationTimeAnalyzer.h"

namespace CppBI = Microsoft::Cpp::BuildInsights;

FunctionCompilationTimeAnalyzer::FunctionCompilationTimeAnalyzer()
	: CppBI::IAnalyzer()
	, m_functionDurations()
{
}

FunctionCompilationTimeAnalyzer::~FunctionCompilationTimeAnalyzer()
{
}

CppBI::AnalysisControl FunctionCompilationTimeAnalyzer::OnStopActivity(const CppBI::EventStack& eventStack)
{
	CppBI::MatchEventInMemberFunction(eventStack.Back(), this, &FunctionCompilationTimeAnalyzer::OnFunction);

	return CppBI::AnalysisControl::CONTINUE;
}

void FunctionCompilationTimeAnalyzer::OnFunction(const CppBI::Activities::Function& function)
{
	// function names are decorated at this stage
	auto result = m_functionDurations.try_emplace(function.Name(), TTimeElapsedPerOccurrence());
	result.first->second.emplace_back(function.Duration());
}
