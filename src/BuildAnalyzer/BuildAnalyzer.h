#pragma once

#include <string>
#include <vector>
#include <chrono>

#include "BuildAnalyzer\Analyzers\FunctionCompilations\FunctionCompilationsAnalyzer.h"
#include "BuildAnalyzer\Analyzers\FileInclusions\FileInclusionsAnalyzer.h"
#include "BuildAnalyzer\Analyzers\FileCompilations\FileCompilationsAnalyzer.h"
#include "BuildAnalyzer\Analyzers\BuildTimeline\BuildTimelineAnalyzer.h"
#include "BuildAnalyzer\Analyzers\TemplateInstantiations\TemplateInstantiationsAnalyzer.h"

namespace Microsoft { namespace Cpp { namespace BuildInsights {
    class IAnalyzer;
}}}

class BuildAnalyzer
{
public:
    enum class TimeDisplayEnum
    {
        Nano,
        Mili,
        Sec,
    };

    struct AnalysisOptions
    {
        std::chrono::milliseconds TimelineIgnoreFunctionsUnder = std::chrono::milliseconds(0);
        std::chrono::milliseconds TimelineIgnoreTemplatesUnder = std::chrono::milliseconds(0);
        bool FunctionCompilations = true;
        bool FileInclusionTimes = true;
        bool FileInclusionGraph = true;
        bool FileCompilations = true;
        bool BuildTimeline = true;
        bool TemplateInstantiations = true;
        TimeDisplayEnum TimeDisplay = TimeDisplayEnum::Nano;
    };

public:
    BuildAnalyzer(const std::string& traceFilePath, const AnalysisOptions& options);
    ~BuildAnalyzer();

    inline bool IsAnalysisPerformed() const { return m_analysisPerformed; }

    bool Analyze();
    bool ExportFunctionCompilationsData(const std::string& path, TimeDisplayEnum timeDisplay) const;
    bool ExportFileInclusionTimesData(const std::string& path, TimeDisplayEnum timeDisplay) const;
    bool ExportFileInclusionGraph(const std::string& path) const;
    bool ExportFileCompilationsData(const std::string& path, TimeDisplayEnum) const;
    bool ExportBuildTimeline(const std::string& path) const;
    bool ExportTemplateInstantiationsData(const std::string& path) const;

private:
    std::string m_traceFilePath;
    AnalysisOptions m_analysisOptions;

    FunctionCompilationsAnalyzer m_functionCompilations;
    FileInclusionsAnalyzer m_fileInclusions;
    FileCompilationsAnalyzer m_fileCompilations;
    BuildTimelineAnalyzer m_buildTimeline;
    TemplateInstantiationsAnalyzer m_templateInstantiations;

    bool m_analysisPerformed;

    std::vector<Microsoft::Cpp::BuildInsights::IAnalyzer*> BuildAnalyzerList(const AnalysisOptions& options);
};
