#include "FileCompilationsAnalyzer.h"

#include <cassert>

#include "AnalysisData\Utilities\CppBuildInsightsDataConversion.h"

FileCompilationsAnalyzer::FileCompilationsAnalyzer()
	: CppBI::IAnalyzer()
	, m_fileCompilationsData()
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
	auto result = m_fileCompilationsData.try_emplace(Utilities::CppBuildInsightsDataConversion::FilePath(GetFilePath(frontEndPass)), FileCompilationData());
	FileCompilationData::Pass& frontEndData = result.first->second.FrontEnd;

	frontEndData.Start = Utilities::CppBuildInsightsDataConversion::Timestamp(frontEndPass.StartTimestamp(), frontEndPass.TickFrequency());
	frontEndData.Stop = Utilities::CppBuildInsightsDataConversion::Timestamp(frontEndPass.StopTimestamp(), frontEndPass.TickFrequency());
}

void FileCompilationsAnalyzer::OnBackEndPassCompleted(const CppBI::Activities::BackEndPass& backEndPass)
{
	auto result = m_fileCompilationsData.try_emplace(Utilities::CppBuildInsightsDataConversion::FilePath(GetFilePath(backEndPass)), FileCompilationData());
	FileCompilationData::Pass& backEndData = result.first->second.BackEnd;

	backEndData.Start = Utilities::CppBuildInsightsDataConversion::Timestamp(backEndPass.StartTimestamp(), backEndPass.TickFrequency());
	backEndData.Stop = Utilities::CppBuildInsightsDataConversion::Timestamp(backEndPass.StopTimestamp(), backEndPass.TickFrequency());
}

const wchar_t* FileCompilationsAnalyzer::GetFilePath(const CppBI::Activities::CompilerPass& compilerPass) const
{
	// apparently, for VS2017 compilations, InputSourcePath can be null
	// in that case, we'll have to rely on OutputObjectPath
	if (compilerPass.InputSourcePath() != nullptr)
	{
		return compilerPass.InputSourcePath();
	}

	assert(compilerPass.OutputObjectPath() != nullptr);
	return compilerPass.OutputObjectPath();
}
