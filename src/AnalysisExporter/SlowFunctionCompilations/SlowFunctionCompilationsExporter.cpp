#include "SlowFunctionCompilationsExporter.h"

#include <Windows.h>
#include <DbgHelp.h>

#include <fstream>

namespace
{
	const DWORD s_undecoratedNameMaxLength = 512;
	const DWORD s_undecorateFlags = UNDNAME_COMPLETE |
									UNDNAME_32_BIT_DECODE |
									UNDNAME_NO_ACCESS_SPECIFIERS;
}

SlowFunctionCompilationsExporter::SlowFunctionCompilationsExporter(const TTimeElapsedPerOcurrencePerConcept& data)
	: m_data(data)
{
}

SlowFunctionCompilationsExporter::~SlowFunctionCompilationsExporter()
{
}

bool SlowFunctionCompilationsExporter::ExportTo(const std::string& path) const
{
	std::ofstream out = std::ofstream(path);
	if (!out)
	{
		return false;
	}

	// data header
	out << "Decorated function name" << ";"
		<< "Undecorated function name" << ";"
		<< "Average elapsed time (nanoseconds)" << std::endl;

	// get undecorated name here
	char undecoratedFunctionName[s_undecoratedNameMaxLength];

	// each function will have one entry with aggregated data
	for (auto&& pair : m_data)
	{
		// calculate average time
		std::chrono::nanoseconds averageTimeElapsed(0);
		for (auto&& timeElapsed : pair.second)
		{
			averageTimeElapsed += timeElapsed;
		}
		averageTimeElapsed /= pair.second.size();

		// undecorate function name
		DWORD result = UnDecorateSymbolName(pair.first.c_str(), undecoratedFunctionName, s_undecoratedNameMaxLength, s_undecorateFlags);

		// dump to stream
		out << pair.first << ";"
			<< (result != 0 ? undecoratedFunctionName : pair.first) << ";"
			<< averageTimeElapsed.count() << std::endl;
	}

	out.close();
	return true;
}
