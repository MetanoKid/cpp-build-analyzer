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
    // to be exported
    struct DataPerFile
    {
        const std::string* FilePath = nullptr;
        std::chrono::nanoseconds TotalInclusionTime = std::chrono::nanoseconds(0);
        std::chrono::nanoseconds AverageInclusionTime = std::chrono::nanoseconds(0);
        unsigned int Occurrences = 0;
    };

    const TTimeElapsedPerOccurrencePerConcept& m_data;
};
