#pragma once

#include <string>
#include <memory>

class FunctionCompilationTimeAnalyzer;
class FileInclusionTimeAnalyzer;

class BuildAnalyzer
{
public:
	BuildAnalyzer(const std::string& traceFilePath);
	~BuildAnalyzer();

	inline bool IsAnalysisPerformed() const { return m_analysisPerformed; }

	bool Analyze();
	bool ExportFunctionCompilationsData(const std::string& path) const;
	bool ExportFileInclusionsData(const std::string& path) const;

private:
	std::string m_traceFilePath;
	std::unique_ptr<FunctionCompilationTimeAnalyzer> m_functionCompilations;
	std::unique_ptr<FileInclusionTimeAnalyzer> m_fileInclusions;
	bool m_analysisPerformed;
};
