#pragma once

#include <string>

#include "AnalysisData\TimeElapsedPerOccurrencePerConcept.h"

class FunctionCompilationTimeExporter
{
public:
	FunctionCompilationTimeExporter(const TTimeElapsedPerOcurrencePerConcept& data);
	~FunctionCompilationTimeExporter();

	// exports to CSV format
	bool ExportTo(const std::string& path) const;

private:
	const TTimeElapsedPerOcurrencePerConcept& m_data;
};
