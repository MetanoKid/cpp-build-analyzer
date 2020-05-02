#include "FunctionCompilationsAnalyzer.h"

FunctionCompilationsAnalyzer::FunctionCompilationsAnalyzer()
    : CppBI::IAnalyzer()
    , m_functionCompilationsData()
{
}

FunctionCompilationsAnalyzer::~FunctionCompilationsAnalyzer()
{
}

CppBI::AnalysisControl FunctionCompilationsAnalyzer::OnStopActivity(const CppBI::EventStack& eventStack)
{
    CppBI::MatchEventInMemberFunction(eventStack.Back(), this, &FunctionCompilationsAnalyzer::OnFunctionCompiled);

    return CppBI::AnalysisControl::CONTINUE;
}

void FunctionCompilationsAnalyzer::OnFunctionCompiled(const CppBI::Activities::Function& function)
{
    // function names are decorated at this stage
    auto result = m_functionCompilationsData.try_emplace(function.Name(), TTimeElapsedPerOccurrence());
    result.first->second.emplace_back(function.Duration());
}
