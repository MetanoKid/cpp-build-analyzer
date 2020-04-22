#pragma once

#include <string>
#include <chrono>

#include "AnalysisData\TimeElapsedPerOccurrencePerConcept.h"

class FileInclusionsExporter
{
public:
	FileInclusionsExporter(const TTimeElapsedPerOccurrencePerConcept& data);
	~FileInclusionsExporter();

	// exports to CSV format
	bool ExportTo(const std::string& path) const;

private:
	// to be exported
	struct DataPerFile
	{
		const std::string* filePath;
		std::chrono::nanoseconds averageInclusionTime;
		unsigned int occurrences;

		DataPerFile(const std::string* filePath,
					const std::chrono::nanoseconds& averageInclusionTime,
					unsigned int occurrences)
			: filePath(filePath)
			, averageInclusionTime(averageInclusionTime)
			, occurrences(occurrences)
		{
		}
	};

	const TTimeElapsedPerOccurrencePerConcept& m_data;
};
