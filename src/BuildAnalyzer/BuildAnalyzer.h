#pragma once

#include <string>
#include <memory>

class FunctionCompilationTimeAnalyzer;

class BuildAnalyzer
{
public:
	BuildAnalyzer(const std::string& traceFilePath);
	~BuildAnalyzer();

	bool Analyze();
	bool ExportSlowFunctionCompilations(const std::string& path) const;

	inline bool IsAnalysisPerformed() const { return m_analysisPerformed; }

private:
	std::string m_traceFilePath;
	std::unique_ptr<FunctionCompilationTimeAnalyzer> m_slowFunctionCompilations;
	bool m_analysisPerformed;
};
