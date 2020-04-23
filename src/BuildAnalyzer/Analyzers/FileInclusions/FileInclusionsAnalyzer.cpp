#include "FileInclusionsAnalyzer.h"

FileInclusionsAnalyzer::FileInclusionsAnalyzer()
	: CppBI::IAnalyzer()
	, m_fileInclusionTimes()
	, m_fileInclusionsPerFile()
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
	const std::string includedFilePath = includedFile.Path();

	// store timing
	{
		auto result = m_fileInclusionTimes.try_emplace(includedFilePath, TTimeElapsedPerOccurrence());
		result.first->second.emplace_back(includedFile.Duration());
	}

	// store inclusion relationship
	{
		auto result = m_fileInclusionsPerFile.try_emplace(fromFile.Path(), TFileInclusions());
		result.first->second.insert(includedFilePath);
	}
}
