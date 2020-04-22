#include "FileInclusionsAnalyzer.h"

FileInclusionsAnalyzer::FileInclusionsAnalyzer()
	: CppBI::IAnalyzer()
	, m_fileInclusionsData()
{
}

FileInclusionsAnalyzer::~FileInclusionsAnalyzer()
{
}

CppBI::AnalysisControl FileInclusionsAnalyzer::OnStopActivity(const CppBI::EventStack& eventStack)
{
	CppBI::MatchEventStackInMemberFunction(eventStack, this, &FileInclusionsAnalyzer::OnFileParsed);

	return CppBI::AnalysisControl::CONTINUE;
}

void FileInclusionsAnalyzer::OnFileParsed(const CppBI::Activities::FrontEndFile& fromFile,
											 const CppBI::Activities::FrontEndFile& includedFile)
{
	auto result = m_fileInclusionsData.try_emplace(includedFile.Path(), TTimeElapsedPerOccurrence());
	result.first->second.emplace_back(includedFile.Duration());
}
