#pragma once

#include <string>
#include <chrono>

#include "AnalysisData\TimeElapsedPerOccurrencePerConcept.h"

class FunctionCompilationsExporter
{
public:
	FunctionCompilationsExporter(const TTimeElapsedPerOcurrencePerConcept& data);
	~FunctionCompilationsExporter();

	// exports to CSV format
	bool ExportTo(const std::string& path) const;

private:
	// to be exported
	struct DataPerFunction
	{
		const std::string* functionName;
		std::chrono::nanoseconds averageCompilationTime;
		unsigned int occurrences;

		DataPerFunction(const std::string* functionName,
						const std::chrono::nanoseconds& averageCompilationTime,
						unsigned int occurrences)
			: functionName(functionName)
			, averageCompilationTime(averageCompilationTime)
			, occurrences(occurrences)
		{
		}
	};

	const TTimeElapsedPerOcurrencePerConcept& m_data;
};
