#pragma once

#include <string>

#include "AnalysisData\TimeElapsedPerOccurrencePerConcept.h"

class SlowFunctionCompilationsExporter
{
public:
	SlowFunctionCompilationsExporter(const TTimeElapsedPerOcurrencePerConcept& data);
	~SlowFunctionCompilationsExporter();

	// exports to CSV format
	bool ExportTo(const std::string& path) const;

private:
	const TTimeElapsedPerOcurrencePerConcept& m_data;
};
