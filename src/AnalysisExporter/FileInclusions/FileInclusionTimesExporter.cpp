#include "FileInclusionTimesExporter.h"

#include <fstream>
#include <algorithm>

namespace
{
    struct DataPerFile
    {
        const std::string* FilePath = nullptr;
        std::chrono::nanoseconds AverageInclusionTime = std::chrono::nanoseconds(0);
        std::chrono::nanoseconds MinimumInclusionTime = std::chrono::nanoseconds(0);
        std::chrono::nanoseconds MaximumInclusionTime = std::chrono::nanoseconds(0);
        std::chrono::nanoseconds StandardDeviation = std::chrono::nanoseconds(0);
        unsigned int Occurrences = 0;
    };
}

FileInclusionTimesExporter::FileInclusionTimesExporter(const TTimeElapsedPerOccurrencePerConcept& data)
    : m_data(data)
{
}

FileInclusionTimesExporter::~FileInclusionTimesExporter()
{
}

template<typename T>
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
        DataPerFile data;
        data.FilePath = &pair.first;
        data.Occurrences = static_cast<unsigned int>(pair.second.size());
        data.MinimumInclusionTime = pair.second[0];
        data.MaximumInclusionTime = pair.second[0];

        for (auto&& timeElapsed : pair.second)
        {
            data.AverageInclusionTime += timeElapsed;

            if (timeElapsed < data.MinimumInclusionTime)
            {
                data.MinimumInclusionTime = timeElapsed;
            }
            
            if (timeElapsed > data.MaximumInclusionTime)
            {
                data.MaximumInclusionTime = timeElapsed;
            }
        }
        data.AverageInclusionTime /= pair.second.size();

        double standardDeviation = 0.0;
        for (auto&& timeElapsed : pair.second)
        {
            std::chrono::nanoseconds differenceWithAverage = timeElapsed - data.AverageInclusionTime;
            standardDeviation += pow(differenceWithAverage.count(), 2);
        }
        standardDeviation /= pair.second.size();
        data.StandardDeviation = std::chrono::nanoseconds(static_cast<std::chrono::nanoseconds::rep>(sqrt(standardDeviation)));

        // store data
        dataPerFile.emplace_back(data);
    }

    // sort entries
    std::sort(dataPerFile.begin(), dataPerFile.end(), [](const DataPerFile& lhs, const DataPerFile& rhs)
    {
        // slowest inclusions first
        return lhs.AverageInclusionTime > rhs.AverageInclusionTime;
    });


    std::string timeType = "nanoseconds";

    if (std::is_same<T, std::chrono::seconds>::value)
    {
        timeType = "seconds";
    }
    else if (std::is_same<T, std::chrono::milliseconds>::value)
    {
        timeType = "miliseconds";
    }

    // write data header to stream
    out << "File path" << ";"
        << "Average elapsed time (" << timeType << ")" << ";"
        << "Minimum elapsed time (" << timeType << ")" << ";"
        << "Maximum elapsed time (" << timeType << ")" << ";"
        << "Standard deviation (" << timeType << ")" << ";"
        << "Occurrences" << std::endl;

    // write data to stream
    for (auto&& data : dataPerFile)
    {
        out << (*data.FilePath) << ";"
            << std::chrono::duration_cast<T>(data.AverageInclusionTime).count() << ";"
            << std::chrono::duration_cast<T>(data.MinimumInclusionTime).count() << ";"
            << std::chrono::duration_cast<T>(data.MaximumInclusionTime).count() << ";"
            << std::chrono::duration_cast<T>(data.StandardDeviation).count() << ";"
            << data.Occurrences << std::endl;
    }

    out.close();
    return true;
}

template
bool FileInclusionTimesExporter::ExportTo<std::chrono::seconds>(const std::string& path) const;

template
bool FileInclusionTimesExporter::ExportTo<std::chrono::milliseconds>(const std::string& path) const;

template
bool FileInclusionTimesExporter::ExportTo<std::chrono::nanoseconds>(const std::string& path) const;

