#include "FunctionCompilationsExporter.h"

#include <fstream>
#include <algorithm>

#include "AnalysisData\Utilities\CppBuildInsightsDataConversion.h"

FunctionCompilationsExporter::FunctionCompilationsExporter(const TTimeElapsedPerOccurrencePerConcept& data)
    : m_data(data)
{
}

FunctionCompilationsExporter::~FunctionCompilationsExporter()
{
}

bool FunctionCompilationsExporter::ExportTo(const std::string& path) const
{
    std::ofstream out = std::ofstream(path);
    if (!out)
    {
        return false;
    }

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
        const std::chrono::nanoseconds totalTimeElapsed = averageTimeElapsed;
        averageTimeElapsed /= pair.second.size();

        // store data
        dataPerFunction.emplace_back(&pair.first,
                                     totalTimeElapsed,
                                     averageTimeElapsed,
                                     static_cast<unsigned int>(pair.second.size()));
    }

    // sort entries
    std::sort(dataPerFunction.begin(), dataPerFunction.end(), [](const DataPerFunction& lhs, const DataPerFunction& rhs)
    {
        // slowest functions first
        return lhs.averageCompilationTime > rhs.averageCompilationTime;
    });

    // write data header to stream
    out << "Decorated function name" << ";"
        << "Undecorated function name" << ";"
        << "Total elapsed time (nanoseconds)" << ";"
        << "Average elapsed time (nanoseconds)" << ";"
        << "Occurrences" << std::endl;

    // write data to file
    for (auto&& data : dataPerFunction)
    {
        // dump to stream
        out << (*data.functionName) << ";"
            << Utilities::CppBuildInsightsDataConversion::UndecorateFunction(*data.functionName) << ";"
            << data.totalCompilationTime.count() << ";"
            << data.averageCompilationTime.count() << ";"
            << data.occurrences << std::endl;
    }

    out.close();
    return true;
}
