#include "FileCompilationsAnalyzer.h"

#include <locale>
#include <codecvt>

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
	auto result = m_fileCompilationsData.try_emplace(ConvertFilePath(frontEndPass.InputSourcePath()), FileCompilationData());
	FileCompilationData::Pass& frontEndData = result.first->second.FrontEnd;

	frontEndData.Start = ConvertActivityTimestamp(frontEndPass.StartTimestamp(), frontEndPass.TickFrequency());
	frontEndData.Stop = ConvertActivityTimestamp(frontEndPass.StopTimestamp, frontEndPass.TickFrequency());
}

void FileCompilationsAnalyzer::OnBackEndPassCompleted(const CppBI::Activities::BackEndPass& backEndPass)
{
	auto result = m_fileCompilationsData.try_emplace(ConvertFilePath(backEndPass.InputSourcePath()), FileCompilationData());
	FileCompilationData::Pass& backEndData = result.first->second.BackEnd;

	backEndData.Start = ConvertActivityTimestamp(backEndPass.StartTimestamp(), backEndPass.TickFrequency());
	backEndData.Stop = ConvertActivityTimestamp(backEndPass.StopTimestamp, backEndPass.TickFrequency());
}

std::string FileCompilationsAnalyzer::ConvertFilePath(const wchar_t* filePath) const
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.to_bytes(filePath);
}

std::chrono::nanoseconds FileCompilationsAnalyzer::ConvertActivityTimestamp(long long timestamp, long long tickFrequency) const
{
	long long convertedTicks = CppBI::Internal::ConvertTickPrecision(timestamp, tickFrequency, std::chrono::nanoseconds::period::den);
	return std::chrono::nanoseconds(convertedTicks);
}
