#include "BuildAnalyzer.h"

#include <cassert>

#include <CppBuildInsights.hpp>
namespace CppBI = Microsoft::Cpp::BuildInsights;

#include "Analyzers\FunctionCompilations\FunctionCompilationsAnalyzer.h"
#include "Analyzers\FileInclusions\FileInclusionsAnalyzer.h"
#include "Analyzers\FileCompilations\FileCompilationsAnalyzer.h"
#include "Analyzers\BuildTimeline\BuildTimelineAnalyzer.h"

#include "AnalysisExporter\FunctionCompilations\FunctionCompilationsExporter.h"
#include "AnalysisExporter\FileInclusions\FileInclusionTimesExporter.h"
#include "AnalysisExporter\FileInclusions\FileInclusionGraphExporter.h"
#include "AnalysisExporter\FileCompilations\FileCompilationsExporter.h"
#include "AnalysisExporter\BuildTimeline\BuildTimelineExporter.h"

namespace
{
	const unsigned int s_numberOfPasses = 1;
}

BuildAnalyzer::BuildAnalyzer(const std::string& traceFilePath, const AnalysisOptions& analysisOptions)
	: m_traceFilePath(traceFilePath)
	, m_analysisOptions(analysisOptions)
	, m_functionCompilations(std::make_unique<FunctionCompilationsAnalyzer>())
	, m_fileInclusions(std::make_unique<FileInclusionsAnalyzer>())
	, m_fileCompilations(std::make_unique<FileCompilationsAnalyzer>())
	, m_buildTimeline(std::make_unique<BuildTimelineAnalyzer>())
	, m_analysisPerformed(false)
{
}

BuildAnalyzer::~BuildAnalyzer()
{
}

bool BuildAnalyzer::Analyze()
{
	assert(!m_analysisPerformed);

	// create list of analyzers based on options
	std::vector<CppBI::IAnalyzer*> analyzers = BuildAnalyzerList(m_analysisOptions);
	assert(analyzers.size() > 0);

	auto dynamicAnalyzerGroup = CppBI::MakeDynamicAnalyzerGroup(analyzers);
	auto analyzerGroup = CppBI::MakeStaticAnalyzerGroup(&dynamicAnalyzerGroup);

	// call C++ Build Insights SDK
	CppBI::RESULT_CODE result = CppBI::Analyze(m_traceFilePath.c_str(), s_numberOfPasses, analyzerGroup);
	m_analysisPerformed = true;

	return result == CppBI::RESULT_CODE::RESULT_CODE_SUCCESS;
}

std::vector<CppBI::IAnalyzer*> BuildAnalyzer::BuildAnalyzerList(const AnalysisOptions& options) const
{
	std::vector<CppBI::IAnalyzer*> analyzers;

	if (options.functionCompilations)
	{
		assert(m_functionCompilations != nullptr);
		analyzers.push_back(m_functionCompilations.get());
	}

	if (options.fileInclusionTimes || options.fileInclusionGraph)
	{
		assert(m_fileInclusions != nullptr);
		analyzers.push_back(m_fileInclusions.get());
	}

	if (options.fileCompilations)
	{
		assert(m_fileCompilations != nullptr);
		analyzers.push_back(m_fileCompilations.get());
	}

	if (options.buildTimeline)
	{
		assert(m_buildTimeline != nullptr);
		analyzers.push_back(m_buildTimeline.get());
	}

	return analyzers;
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

bool BuildAnalyzer::ExportBuildTimeline(const std::string& path) const
{
	assert(m_analysisPerformed);
	assert(m_buildTimeline != nullptr);

	BuildTimelineExporter exporter(m_buildTimeline->GetTimeline());
	return exporter.ExportTo(path);
}
