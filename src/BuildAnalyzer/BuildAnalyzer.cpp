#include "BuildAnalyzer.h"

#include <cassert>

#include <CppBuildInsights.hpp>
namespace CppBI = Microsoft::Cpp::BuildInsights;

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
    , m_functionCompilations()
    , m_fileInclusions()
    , m_fileCompilations()
    , m_buildTimeline()
    , m_filterTimeline(analysisOptions.timelineIgnoreFunctionsUnder)
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

std::vector<CppBI::IAnalyzer*> BuildAnalyzer::BuildAnalyzerList(const AnalysisOptions& options)
{
    std::vector<CppBI::IAnalyzer*> analyzers;

    if (options.functionCompilations)
    {
        analyzers.push_back(&m_functionCompilations);
    }

    if (options.fileInclusionTimes || options.fileInclusionGraph)
    {
        analyzers.push_back(&m_fileInclusions);
    }

    if (options.fileCompilations)
    {
        analyzers.push_back(&m_fileCompilations);
    }

    if (options.buildTimeline)
    {
        analyzers.push_back(&m_buildTimeline);
        analyzers.push_back(&m_filterTimeline);
    }

    return analyzers;
}

bool BuildAnalyzer::ExportFunctionCompilationsData(const std::string& path) const
{
    assert(m_analysisPerformed);

    FunctionCompilationsExporter exporter(m_functionCompilations.GetData());
    return exporter.ExportTo(path);
}

bool BuildAnalyzer::ExportFileInclusionTimesData(const std::string& path) const
{
    assert(m_analysisPerformed);

    FileInclusionTimesExporter exporter(m_fileInclusions.GetTimeData());
    return exporter.ExportTo(path);
}

bool BuildAnalyzer::ExportFileInclusionGraph(const std::string& path) const
{
    assert(m_analysisPerformed);

    FileInclusionGraphExporter exporter(m_fileInclusions.GetFileInclusionsData());
    return exporter.ExportTo(path);
}

bool BuildAnalyzer::ExportFileCompilationsData(const std::string& path) const
{
    assert(m_analysisPerformed);

    FileCompilationsExporter exporter(m_fileCompilations.GetData());
    return exporter.ExportTo(path);
}

bool BuildAnalyzer::ExportBuildTimeline(const std::string& path) const
{
    assert(m_analysisPerformed);

    BuildTimelineExporter exporter(m_buildTimeline.GetTimeline(), m_filterTimeline.GetIgnoredEntries());
    return exporter.ExportTo(path);
}
