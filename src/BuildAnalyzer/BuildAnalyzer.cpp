#include "BuildAnalyzer.h"

#include <cassert>
#include <CppBuildInsights.hpp>

#include "Analyzers\FunctionCompilationTimeAnalyzer.h"
#include "Analyzers\FileInclusionTimeAnalyzer.h"
#include "AnalysisExporter\SlowFunctionCompilations\FunctionCompilationsExporter.h"

namespace
{
	const unsigned int s_numberOfPasses = 1;
}

namespace CppBI = Microsoft::Cpp::BuildInsights;

BuildAnalyzer::BuildAnalyzer(const std::string& traceFilePath)
	: m_traceFilePath(traceFilePath)
	, m_functionCompilations(std::make_unique<FunctionCompilationTimeAnalyzer>())
	, m_fileInclusionTimes(std::make_unique<FileInclusionTimeAnalyzer>())
	, m_analysisPerformed(false)
{
}

BuildAnalyzer::~BuildAnalyzer()
{
}

bool BuildAnalyzer::Analyze()
{
	assert(!m_analysisPerformed);
	assert(m_functionCompilations != nullptr);
	assert(m_fileInclusionTimes != nullptr);

	auto analyzerGroup = CppBI::MakeStaticAnalyzerGroup(m_functionCompilations.get(),
														m_fileInclusionTimes.get());

	CppBI::RESULT_CODE result = CppBI::Analyze(m_traceFilePath.c_str(), s_numberOfPasses, analyzerGroup);
	m_analysisPerformed = true;

	return result == CppBI::RESULT_CODE::RESULT_CODE_SUCCESS;
}

bool BuildAnalyzer::ExportFunctionCompilationTimes(const std::string& path) const
{
	assert(m_analysisPerformed);
	assert(m_functionCompilations != nullptr);

	FunctionCompilationsExporter exporter(m_functionCompilations->GetData());
	return exporter.ExportTo(path);
}

bool BuildAnalyzer::ExportFileInclusionTimes(const std::string& path) const
{
	assert(m_analysisPerformed);
	assert(m_fileInclusionTimes != nullptr);

	return false;
}
