#include "SlowFunctionCompilationAnalyzer.h"

namespace CppBI = Microsoft::Cpp::BuildInsights;

SlowFunctionCompilationAnalyzer::SlowFunctionCompilationAnalyzer()
	: CppBI::IAnalyzer()
	, m_functionDurations()
{
}

SlowFunctionCompilationAnalyzer::~SlowFunctionCompilationAnalyzer()
{
}

CppBI::AnalysisControl SlowFunctionCompilationAnalyzer::OnStopActivity(const CppBI::EventStack& eventStack)
{
	CppBI::MatchEventInMemberFunction(eventStack.Back(), this, &SlowFunctionCompilationAnalyzer::OnFunction);

	return CppBI::AnalysisControl::CONTINUE;
}

void SlowFunctionCompilationAnalyzer::OnFunction(const CppBI::Activities::Function& function)
{
	// function names are decorated at this stage
	auto result = m_functionDurations.try_emplace(function.Name(), TDurations());
	result.first->second.emplace_back(function.Duration());
}
