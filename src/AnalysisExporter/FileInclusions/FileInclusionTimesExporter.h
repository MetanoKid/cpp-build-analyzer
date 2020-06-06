#pragma once

#include <string>
#include <chrono>

#include "AnalysisData\TimeElapsedPerOccurrencePerConcept.h"

class FileInclusionTimesExporter
{
public:
    FileInclusionTimesExporter(const TTimeElapsedPerOccurrencePerConcept& data);
    ~FileInclusionTimesExporter();

    // exports to CSV format
    bool ExportTo(const std::string& path) const;

private:
    const TTimeElapsedPerOccurrencePerConcept& m_data;
};
