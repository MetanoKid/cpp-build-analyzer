#include "FileInclusionTimeAnalyzer.h"

FileInclusionTimeAnalyzer::FileInclusionTimeAnalyzer()
	: CppBI::IAnalyzer()
	, m_inclusionDurations()
{
}

FileInclusionTimeAnalyzer::~FileInclusionTimeAnalyzer()
{
}

CppBI::AnalysisControl FileInclusionTimeAnalyzer::OnStopActivity(const CppBI::EventStack& eventStack)
{
	CppBI::MatchEventStackInMemberFunction(eventStack, this, &FileInclusionTimeAnalyzer::OnFileParsed);

	return CppBI::AnalysisControl::CONTINUE;
}

void FileInclusionTimeAnalyzer::OnFileParsed(const CppBI::Activities::FrontEndFile& fromFile,
											 const CppBI::Activities::FrontEndFile& includedFile)
{
	auto result = m_inclusionDurations.try_emplace(includedFile.Path(), TTimeElapsedPerOccurrence());
	result.first->second.emplace_back(includedFile.Duration());
}
