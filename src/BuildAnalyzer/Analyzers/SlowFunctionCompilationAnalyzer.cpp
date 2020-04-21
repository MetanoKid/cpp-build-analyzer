#include "SlowFunctionCompilationAnalyzer.h"

namespace CppBI = Microsoft::Cpp::BuildInsights;

SlowFunctionCompilationAnalyzer::SlowFunctionCompilationAnalyzer()
	: CppBI::IAnalyzer()
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
	// TODO: store it somehow!
}
