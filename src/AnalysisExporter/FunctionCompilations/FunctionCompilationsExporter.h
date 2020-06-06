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
    const TTimeElapsedPerOccurrencePerConcept& m_data;
};
