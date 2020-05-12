#pragma once

#include <string>

#include "AnalysisData\BuildTimeline\IgnoredEntries.h"

class BuildTimeline;
class TimelineEntry;
class ProcessIdThreadIdRecalculation;

class BuildTimelineExporter
{
public:
    BuildTimelineExporter(const BuildTimeline& timeline, const TIgnoredEntries& ignoredEntries);
    ~BuildTimelineExporter();

    // exports to Google Chrome trace format
    bool ExportTo(const std::string& path) const;

private:
    const BuildTimeline& m_timeline;
    const TIgnoredEntries& m_ignoredEntries;
};
