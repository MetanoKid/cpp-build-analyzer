#include "FileCompilationsAnalyzer.h"

FileCompilationsAnalyzer::FileCompilationsAnalyzer()
	: CppBI::IAnalyzer()
{
}

FileCompilationsAnalyzer::~FileCompilationsAnalyzer()
{
}

CppBI::AnalysisControl FileCompilationsAnalyzer::OnStopActivity(const CppBI::EventStack& eventStack)
{
	bool processed = CppBI::MatchEventInMemberFunction(eventStack.Back(), this, &FileCompilationsAnalyzer::OnFrontEndPassCompleted) ||
					 CppBI::MatchEventInMemberFunction(eventStack.Back(), this, &FileCompilationsAnalyzer::OnBackEndPassCompleted);
	return CppBI::AnalysisControl::CONTINUE;
}

void FileCompilationsAnalyzer::OnFrontEndPassCompleted(const CppBI::Activities::FrontEndPass& frontEndPass)
{
	// TODO: pair both together
}

void FileCompilationsAnalyzer::OnBackEndPassCompleted(const CppBI::Activities::BackEndPass& backEndPass)
{
	// TODO: pair both together
}
