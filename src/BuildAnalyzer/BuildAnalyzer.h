#pragma once

#include <string>
#include <vector>

#include "BuildAnalyzer\Analyzers\FunctionCompilations\FunctionCompilationsAnalyzer.h"
#include "BuildAnalyzer\Analyzers\FileInclusions\FileInclusionsAnalyzer.h"
#include "BuildAnalyzer\Analyzers\FileCompilations\FileCompilationsAnalyzer.h"
#include "BuildAnalyzer\Analyzers\BuildTimeline\BuildTimelineAnalyzer.h"

namespace Microsoft { namespace Cpp { namespace BuildInsights {
    class IAnalyzer;
}}}

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

    FunctionCompilationsAnalyzer m_functionCompilations;
    FileInclusionsAnalyzer m_fileInclusions;
    FileCompilationsAnalyzer m_fileCompilations;
    BuildTimelineAnalyzer m_buildTimeline;

    bool m_analysisPerformed;

    std::vector<Microsoft::Cpp::BuildInsights::IAnalyzer*> BuildAnalyzerList(const AnalysisOptions& options);
};
