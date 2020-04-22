#include "FileCompilationAnalyzer.h"

FileCompilationAnalyzer::FileCompilationAnalyzer()
	: CppBI::IAnalyzer()
{
}

FileCompilationAnalyzer::~FileCompilationAnalyzer()
{
}

CppBI::AnalysisControl FileCompilationAnalyzer::OnStopActivity(const CppBI::EventStack& eventStack)
{
	bool processed = CppBI::MatchEventInMemberFunction(eventStack.Back(), this, &FileCompilationAnalyzer::OnFrontEndPassCompleted) ||
					 CppBI::MatchEventInMemberFunction(eventStack.Back(), this, &FileCompilationAnalyzer::OnBackEndPassCompleted);
	return CppBI::AnalysisControl::CONTINUE;
}

void FileCompilationAnalyzer::OnFrontEndPassCompleted(const CppBI::Activities::FrontEndPass& frontEndPass)
{
	// TODO: pair both together
}

void FileCompilationAnalyzer::OnBackEndPassCompleted(const CppBI::Activities::BackEndPass& backEndPass)
{
	// TODO: pair both together
}
