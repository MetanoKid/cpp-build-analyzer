#include "BuildAnalyzer.h"

#include <cassert>
#include <CppBuildInsights.hpp>

#include "Analyzers\FunctionCompilationTimeAnalyzer.h"
#include "AnalysisExporter\SlowFunctionCompilations\FunctionCompilationTimeExporter.h"

namespace
{
	const unsigned int s_numberOfPasses = 1;
}

namespace CppBI = Microsoft::Cpp::BuildInsights;

BuildAnalyzer::BuildAnalyzer(const std::string& traceFilePath)
	: m_traceFilePath(traceFilePath)
	, m_slowFunctionCompilations(std::make_unique<FunctionCompilationTimeAnalyzer>())
	, m_analysisPerformed(false)
{
}

BuildAnalyzer::~BuildAnalyzer()
{
}

bool BuildAnalyzer::Analyze()
{
	assert(!m_analysisPerformed);
	assert(m_slowFunctionCompilations != nullptr);

	auto analyzerGroup = CppBI::MakeStaticAnalyzerGroup(m_slowFunctionCompilations.get());

	CppBI::RESULT_CODE result = CppBI::Analyze(m_traceFilePath.c_str(), s_numberOfPasses, analyzerGroup);
	m_analysisPerformed = true;

	return result == CppBI::RESULT_CODE::RESULT_CODE_SUCCESS;
}

bool BuildAnalyzer::ExportSlowFunctionCompilations(const std::string& path) const
{
	assert(m_analysisPerformed);
	assert(m_slowFunctionCompilations != nullptr);

	FunctionCompilationTimeExporter exporter(m_slowFunctionCompilations->GetFunctionDurations());
	return exporter.ExportTo(path);
}
