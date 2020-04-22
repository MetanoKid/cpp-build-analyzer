#pragma once

#include <string>
#include <chrono>

#include <CppBuildInsights.hpp>
namespace CppBI = Microsoft::Cpp::BuildInsights;

#include "AnalysisData\FileCompilationDataPerFile.h"

class FileCompilationsAnalyzer : public CppBI::IAnalyzer
{
public:
	FileCompilationsAnalyzer();
	virtual ~FileCompilationsAnalyzer();

	CppBI::AnalysisControl OnStopActivity(const CppBI::EventStack& eventStack) override;

	inline const TFileCompilationDataPerFile& GetData() const { return m_fileCompilationsData; }

private:
	TFileCompilationDataPerFile m_fileCompilationsData;

	void OnFrontEndPassCompleted(const CppBI::Activities::FrontEndPass& frontEndPass);
	void OnBackEndPassCompleted(const CppBI::Activities::BackEndPass& backEndPass);

	std::string ConvertFilePath(const wchar_t* filePath) const;
	std::chrono::nanoseconds ConvertActivityTimestamp(long long timestamp, long long tickFrequency) const;
};
