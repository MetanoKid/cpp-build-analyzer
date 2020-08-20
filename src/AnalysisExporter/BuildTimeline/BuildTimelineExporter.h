#pragma once

#include <string>

class BuildTimeline;
class TimelineEntry;
class ProcessIdThreadIdRecalculation;

class BuildTimelineExporter
{
public:
    BuildTimelineExporter(const BuildTimeline& timeline);
    ~BuildTimelineExporter();

    // exports to Google Chrome trace format
    bool ExportTo(const std::string& path) const;

private:
    const BuildTimeline& m_timeline;
};
