#include "FunctionCompilationTimeAnalyzer.h"

namespace CppBI = Microsoft::Cpp::BuildInsights;

FunctionCompilationTimeAnalyzer::FunctionCompilationTimeAnalyzer()
	: CppBI::IAnalyzer()
	, m_functionCompilationsData()
{
}

FunctionCompilationTimeAnalyzer::~FunctionCompilationTimeAnalyzer()
{
}

CppBI::AnalysisControl FunctionCompilationTimeAnalyzer::OnStopActivity(const CppBI::EventStack& eventStack)
{
	CppBI::MatchEventInMemberFunction(eventStack.Back(), this, &FunctionCompilationTimeAnalyzer::OnFunctionCompiled);

	return CppBI::AnalysisControl::CONTINUE;
}

void FunctionCompilationTimeAnalyzer::OnFunctionCompiled(const CppBI::Activities::Function& function)
{
	// function names are decorated at this stage
	auto result = m_functionCompilationsData.try_emplace(function.Name(), TTimeElapsedPerOccurrence());
	result.first->second.emplace_back(function.Duration());
}
