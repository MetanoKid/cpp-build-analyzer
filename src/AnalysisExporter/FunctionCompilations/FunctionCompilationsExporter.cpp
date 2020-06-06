#include "FunctionCompilationsExporter.h"

#include <fstream>
#include <algorithm>

#include "AnalysisData\Utilities\CppBuildInsightsDataConversion.h"

namespace
{
    struct DataPerFunction
    {
        const std::string* FunctionName = nullptr;
        std::chrono::nanoseconds AverageCompilationTime = std::chrono::nanoseconds(0);
        std::chrono::nanoseconds MinimumCompilationTime = std::chrono::nanoseconds(0);
        std::chrono::nanoseconds MaximumCompilationTime = std::chrono::nanoseconds(0);
        std::chrono::nanoseconds StandardDeviation = std::chrono::nanoseconds(0);
        unsigned int Occurrences = 0;
    };
}

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
        DataPerFunction data;
        data.FunctionName = &pair.first;
        data.Occurrences = static_cast<unsigned int>(pair.second.size());
        data.MinimumCompilationTime = pair.second[0];
        data.MaximumCompilationTime = pair.second[0];

        for (auto&& timeElapsed : pair.second)
        {
            data.AverageCompilationTime += timeElapsed;

            if (timeElapsed < data.MinimumCompilationTime)
            {
                data.MinimumCompilationTime = timeElapsed;
            }

            if (timeElapsed > data.MaximumCompilationTime)
            {
                data.MaximumCompilationTime = timeElapsed;
            }
        }
        data.AverageCompilationTime /= pair.second.size();

        double standardDeviation = 0.0;
        for (auto&& timeElapsed : pair.second)
        {
            std::chrono::nanoseconds differenceWithAverage = timeElapsed - data.AverageCompilationTime;
            standardDeviation += pow(differenceWithAverage.count(), 2);
        }
        standardDeviation /= pair.second.size();
        data.StandardDeviation = std::chrono::nanoseconds(static_cast<std::chrono::nanoseconds::rep>(sqrt(standardDeviation)));

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
        << "Average elapsed time (nanoseconds)" << ";"
        << "Minimum elapsed time (nanoseconds)" << ";"
        << "Maximum elapsed time (nanoseconds)" << ";"
        << "Standard deviation (nanoseconds)" << ";"
        << "Occurrences" << std::endl;

    // write data to file
    for (auto&& data : dataPerFunction)
    {
        // dump to stream
        out << (*data.FunctionName) << ";"
            << Utilities::CppBuildInsightsDataConversion::UndecorateFunction(*data.FunctionName) << ";"
            << data.AverageCompilationTime.count() << ";"
            << data.MinimumCompilationTime.count() << ";"
            << data.MaximumCompilationTime.count() << ";"
            << data.StandardDeviation.count() << ";"
            << data.Occurrences << std::endl;
    }

    out.close();
    return true;
}
