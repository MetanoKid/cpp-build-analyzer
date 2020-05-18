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

        DataPerFunction data;
        data.FunctionName = &pair.first;
        data.TotalCompilationTime = totalTimeElapsed;
        data.AverageCompilationTime = averageTimeElapsed;
        data.Occurrences = static_cast<unsigned int>(pair.second.size());

        // store data
        dataPerFunction.emplace_back(data);
    }

    // sort entries
    std::sort(dataPerFunction.begin(), dataPerFunction.end(), [](const DataPerFunction& lhs, const DataPerFunction& rhs)
    {
        // slowest functions first
        return lhs.AverageCompilationTime > rhs.AverageCompilationTime;
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
        out << (*data.FunctionName) << ";"
            << Utilities::CppBuildInsightsDataConversion::UndecorateFunction(*data.FunctionName) << ";"
            << data.TotalCompilationTime.count() << ";"
            << data.AverageCompilationTime.count() << ";"
            << data.Occurrences << std::endl;
    }

    out.close();
    return true;
}
