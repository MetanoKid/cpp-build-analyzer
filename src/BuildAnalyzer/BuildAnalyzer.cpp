#include "BuildAnalyzer.h"

#include <cassert>
#include <CppBuildInsights.hpp>

#include "Analyzers\FunctionCompilations\FunctionCompilationsAnalyzer.h"
#include "Analyzers\FileInclusions\FileInclusionsAnalyzer.h"
#include "Analyzers\FileCompilations\FileCompilationsAnalyzer.h"
#include "AnalysisExporter\FunctionCompilations\FunctionCompilationsExporter.h"
#include "AnalysisExporter\FileInclusions\FileInclusionTimesExporter.h"
#include "AnalysisExporter\FileInclusions\FileInclusionGraphExporter.h"
#include "AnalysisExporter\FileCompilations\FileCompilationsExporter.h"

namespace
{
	const unsigned int s_numberOfPasses = 1;
}

namespace CppBI = Microsoft::Cpp::BuildInsights;

BuildAnalyzer::BuildAnalyzer(const std::string& traceFilePath)
	: m_traceFilePath(traceFilePath)
	, m_functionCompilations(std::make_unique<FunctionCompilationsAnalyzer>())
	, m_fileInclusions(std::make_unique<FileInclusionsAnalyzer>())
	, m_fileCompilations(std::make_unique<FileCompilationsAnalyzer>())
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
	assert(m_fileInclusions != nullptr);
	assert(m_fileCompilations != nullptr);

	auto analyzerGroup = CppBI::MakeStaticAnalyzerGroup(m_functionCompilations.get(),
														m_fileInclusions.get(),
														m_fileCompilations.get());

	CppBI::RESULT_CODE result = CppBI::Analyze(m_traceFilePath.c_str(), s_numberOfPasses, analyzerGroup);
	m_analysisPerformed = true;

	return result == CppBI::RESULT_CODE::RESULT_CODE_SUCCESS;
}

bool BuildAnalyzer::ExportFunctionCompilationsData(const std::string& path) const
{
	assert(m_analysisPerformed);
	assert(m_fileInclusions != nullptr);

	FunctionCompilationsExporter exporter(m_functionCompilations->GetData());
	return exporter.ExportTo(path);
}

bool BuildAnalyzer::ExportFileInclusionTimesData(const std::string& path) const
{
	assert(m_analysisPerformed);
	assert(m_fileInclusions != nullptr);

	FileInclusionTimesExporter exporter(m_fileInclusions->GetTimeData());
	return exporter.ExportTo(path);
}

bool BuildAnalyzer::ExportFileInclusionGraph(const std::string& path) const
{
	assert(m_analysisPerformed);
	assert(m_fileInclusions != nullptr);

	FileInclusionGraphExporter exporter(m_fileInclusions->GetFileInclusionsData());
	return exporter.ExportTo(path);
}

bool BuildAnalyzer::ExportFileCompilationsData(const std::string& path) const
{
	assert(m_analysisPerformed);
	assert(m_fileCompilations != nullptr);

	FileCompilationsExporter exporter(m_fileCompilations->GetData());
	return exporter.ExportTo(path);
}
