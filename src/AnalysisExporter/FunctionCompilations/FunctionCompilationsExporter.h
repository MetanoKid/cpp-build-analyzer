#pragma once

#include <string>
#include <chrono>

#include "AnalysisData\TimeElapsedPerOccurrencePerConcept.h"

class FunctionCompilationsExporter
{
public:
    FunctionCompilationsExporter(const TTimeElapsedPerOccurrencePerConcept& data);
    ~FunctionCompilationsExporter();

    // exports to CSV format
    bool ExportTo(const std::string& path) const;

private:
    // to be exported
    struct DataPerFunction
    {
        const std::string* FunctionName = nullptr;
        std::chrono::nanoseconds TotalCompilationTime = std::chrono::nanoseconds(0);
        std::chrono::nanoseconds AverageCompilationTime = std::chrono::nanoseconds(0);
        unsigned int Occurrences = 0;
    };

    const TTimeElapsedPerOccurrencePerConcept& m_data;
};
