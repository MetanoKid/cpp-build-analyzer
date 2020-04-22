#pragma once

#include <string>
#include <memory>

class FunctionCompilationsAnalyzer;
class FileInclusionsAnalyzer;
class FileCompilationsAnalyzer;

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

	std::unique_ptr<FunctionCompilationsAnalyzer> m_functionCompilations;
	std::unique_ptr<FileInclusionsAnalyzer> m_fileInclusions;
	std::unique_ptr<FileCompilationsAnalyzer> m_fileCompilations;
	
	bool m_analysisPerformed;
};
