#include "SlowFunctionCompilationsAnalyzer.h"

namespace CppBI = Microsoft::Cpp::BuildInsights;

SlowFunctionCompilationsAnalyzer::SlowFunctionCompilationsAnalyzer()
	: CppBI::IAnalyzer()
	, m_functionDurations()
{
}

SlowFunctionCompilationsAnalyzer::~SlowFunctionCompilationsAnalyzer()
{
}

CppBI::AnalysisControl SlowFunctionCompilationsAnalyzer::OnStopActivity(const CppBI::EventStack& eventStack)
{
	CppBI::MatchEventInMemberFunction(eventStack.Back(), this, &SlowFunctionCompilationsAnalyzer::OnFunction);

	return CppBI::AnalysisControl::CONTINUE;
}

void SlowFunctionCompilationsAnalyzer::OnFunction(const CppBI::Activities::Function& function)
{
	// function names are decorated at this stage
	auto result = m_functionDurations.try_emplace(function.Name(), TDurations());
	result.first->second.emplace_back(function.Duration());
}
