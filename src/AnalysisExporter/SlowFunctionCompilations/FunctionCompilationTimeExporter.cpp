#include "FunctionCompilationTimeExporter.h"

#include <Windows.h>
#include <DbgHelp.h>

#include <fstream>
#include <algorithm>

namespace
{
	const DWORD s_undecoratedNameMaxLength = 512;
	const DWORD s_undecorateFlags = UNDNAME_COMPLETE |
									UNDNAME_32_BIT_DECODE |
									UNDNAME_NO_ACCESS_SPECIFIERS;
}

FunctionCompilationTimeExporter::FunctionCompilationTimeExporter(const TTimeElapsedPerOcurrencePerConcept& data)
	: m_data(data)
{
}

FunctionCompilationTimeExporter::~FunctionCompilationTimeExporter()
{
}

bool FunctionCompilationTimeExporter::ExportTo(const std::string& path) const
{
	std::ofstream out = std::ofstream(path);
	if (!out)
	{
		return false;
	}

	// data header
	out << "Decorated function name" << ";"
		<< "Undecorated function name" << ";"
		<< "Average elapsed time (nanoseconds)" << ";"
		<< "Occurrences" << std::endl;

	// get undecorated name here
	char undecoratedFunctionName[s_undecoratedNameMaxLength];

	// store aggregated data in this vector
	std::vector<DataPerFunction> dataPerFunction;

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

		// store it
		dataPerFunction.emplace_back(&pair.first, averageTimeElapsed, static_cast<unsigned int>(pair.second.size()));
	}

	// sort entries
	std::sort(dataPerFunction.begin(), dataPerFunction.end(), [](const DataPerFunction& lhs, const DataPerFunction& rhs)
	{
		// slowest functions first
		return lhs.averageCompilationTime > rhs.averageCompilationTime;
	});

	// write data to file
	for (auto&& data : dataPerFunction)
	{
		// undecorate function name
		DWORD result = UnDecorateSymbolName(data.functionName->c_str(), undecoratedFunctionName,
											s_undecoratedNameMaxLength, s_undecorateFlags);

		// dump to stream
		out << (*data.functionName) << ";"
			<< (result != 0 ? undecoratedFunctionName : (*data.functionName)) << ";"
			<< data.averageCompilationTime.count() << ";"
			<< data.occurrences << std::endl;
	}

	out.close();
	return true;
}
