#pragma once

#include <string>
#include <memory>
#include <vector>

namespace Microsoft { namespace Cpp { namespace BuildInsights {
	class IAnalyzer;
}}}

class FunctionCompilationsAnalyzer;
class FileInclusionsAnalyzer;
class FileCompilationsAnalyzer;
class BuildTimelineAnalyzer;

class BuildAnalyzer
{
public:
	struct AnalysisOptions
	{
		bool functionCompilations = true;
		bool fileInclusionTimes = true;
		bool fileInclusionGraph = true;
		bool fileCompilations = true;
		bool buildTimeline = true;
	};

public:
	BuildAnalyzer(const std::string& traceFilePath, const AnalysisOptions& options);
	~BuildAnalyzer();

	inline bool IsAnalysisPerformed() const { return m_analysisPerformed; }

	bool Analyze();
	bool ExportFunctionCompilationsData(const std::string& path) const;
	bool ExportFileInclusionTimesData(const std::string& path) const;
	bool ExportFileInclusionGraph(const std::string& path) const;
	bool ExportFileCompilationsData(const std::string& path) const;
	bool ExportBuildTimeline(const std::string& path) const;

private:
	std::string m_traceFilePath;
	AnalysisOptions m_analysisOptions;

	std::unique_ptr<FunctionCompilationsAnalyzer> m_functionCompilations;
	std::unique_ptr<FileInclusionsAnalyzer> m_fileInclusions;
	std::unique_ptr<FileCompilationsAnalyzer> m_fileCompilations;
	std::unique_ptr<BuildTimelineAnalyzer> m_buildTimeline;

	bool m_analysisPerformed;

	std::vector<Microsoft::Cpp::BuildInsights::IAnalyzer*> BuildAnalyzerList(const AnalysisOptions& options) const;
};
