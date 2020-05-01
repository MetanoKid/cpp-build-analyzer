#include "FileInclusionTimesExporter.h"

#include <fstream>
#include <algorithm>

FileInclusionTimesExporter::FileInclusionTimesExporter(const TTimeElapsedPerOccurrencePerConcept& data)
	: m_data(data)
{
}

FileInclusionTimesExporter::~FileInclusionTimesExporter()
{
}

bool FileInclusionTimesExporter::ExportTo(const std::string& path) const
{
	std::ofstream out(path);
	if (!out)
	{
		return false;
	}

	// store aggregated data in this vector
	std::vector<DataPerFile> dataPerFile;

	for (auto&& pair : m_data)
	{
		// calculate average time
		std::chrono::nanoseconds averageTimeElapsed(0);
		for (auto&& timeElapsed : pair.second)
		{
			averageTimeElapsed += timeElapsed;
		}
		std::chrono::nanoseconds totalTimeElapsed = averageTimeElapsed;
		averageTimeElapsed /= pair.second.size();

		// store data
		dataPerFile.emplace_back(&pair.first,
								 totalTimeElapsed,
								 averageTimeElapsed,
								 static_cast<unsigned int>(pair.second.size()));
	}

	// sort entries
	std::sort(dataPerFile.begin(), dataPerFile.end(), [](const DataPerFile& lhs, const DataPerFile& rhs)
	{
		// slowest inclusions first
		return lhs.totalInclusionTime > rhs.totalInclusionTime;
	});

	// write data header to stream
	out << "File path" << ";"
		<< "Total elapsed time (nanoseconds)" << ";"
		<< "Average elapsed time (nanoseconds)" << ";"
		<< "Occurrences" << std::endl;

	// write data to stream
	for (auto&& data : dataPerFile)
	{
		out << (*data.filePath) << ";"
			<< data.totalInclusionTime.count() << ";"
			<< data.averageInclusionTime.count() << ";"
			<< data.occurrences << std::endl;
	}

	out.close();
	return true;
}
