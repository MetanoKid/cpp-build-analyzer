#include "TemplateInstantiationsExporter.h"

#include <fstream>
#include <algorithm>
#include <cassert>

namespace
{
    struct DataPerTemplate
    {
        const std::string* PrimaryTemplateKey = nullptr;
        std::chrono::nanoseconds AverageInstantiationTime = std::chrono::nanoseconds(0);
        std::chrono::nanoseconds MinimumInstantiationTime = std::chrono::nanoseconds(0);
        std::chrono::nanoseconds MaximumInstantiationTime = std::chrono::nanoseconds(0);
        std::chrono::nanoseconds StandardDeviation = std::chrono::nanoseconds(0);
        std::vector<std::chrono::nanoseconds> InstantiationTimes;
    };
}

TemplateInstantiationsExporter::TemplateInstantiationsExporter(const TSymbolNames& symbolNames,
                                                               const TTemplateInstantiationDataPerOccurrence& templateInstantiations)
    : m_symbolNames(symbolNames)
    , m_templateInstantiations(templateInstantiations)
{
}

TemplateInstantiationsExporter::~TemplateInstantiationsExporter()
{
}

bool TemplateInstantiationsExporter::ExportTo(const std::string& path) const
{
    std::ofstream out = std::ofstream(path);
    if (!out)
    {
        return false;
    }

    // aggregate data
    typedef std::unordered_map<std::string, DataPerTemplate> TAggregatedData;
    TAggregatedData aggregatedData;
    for (auto&& pair : m_templateInstantiations)
    {
        const std::chrono::nanoseconds& timeElapsed = pair.second.Duration;

        auto itSpecializationTemplateName = m_symbolNames.find(pair.second.Specialization);
        assert(itSpecializationTemplateName != m_symbolNames.end());

        // from the docs: "Comparing types between different compiler front-end passes requires using symbol names"
        auto result = aggregatedData.try_emplace(itSpecializationTemplateName->second, DataPerTemplate());
        auto& data = result.first->second;
        
        // first occurrence?
        if (result.second)
        {
            // only need to point to one of them, as they should be using the same string
            auto itPrimaryTemplateName = m_symbolNames.find(pair.second.Primary);
            assert(itPrimaryTemplateName != m_symbolNames.end());

            data.PrimaryTemplateKey = &itPrimaryTemplateName->second;
        }

        data.InstantiationTimes.push_back(timeElapsed);
    }

    // perform final calculations on aggregated data
    for (auto&& pair : aggregatedData)
    {
        pair.second.MinimumInstantiationTime = pair.second.InstantiationTimes[0];
        pair.second.MaximumInstantiationTime = pair.second.InstantiationTimes[0];

        for (auto&& timeElapsed : pair.second.InstantiationTimes)
        {
            pair.second.AverageInstantiationTime += timeElapsed;

            if (timeElapsed < pair.second.MinimumInstantiationTime)
            {
                pair.second.MinimumInstantiationTime = timeElapsed;
            }

            if (timeElapsed > pair.second.MaximumInstantiationTime)
            {
                pair.second.MaximumInstantiationTime = timeElapsed;
            }
        }
        pair.second.AverageInstantiationTime /= pair.second.InstantiationTimes.size();

        double standardDeviation = 0.0;
        for (auto&& timeElapsed : pair.second.InstantiationTimes)
        {
            std::chrono::nanoseconds differenceWithAverage = timeElapsed - pair.second.AverageInstantiationTime;
            standardDeviation += pow(differenceWithAverage.count(), 2);
        }
        standardDeviation /= pair.second.InstantiationTimes.size();
        pair.second.StandardDeviation = std::chrono::nanoseconds(static_cast<std::chrono::nanoseconds::rep>(sqrt(standardDeviation)));
    }

    // sort aggregated data
    typedef std::pair<const std::string*, const DataPerTemplate*> DataPerSpecialization;
    std::vector<const TAggregatedData::value_type*> sortedAggregatedData;
    for (auto&& pair : aggregatedData)
    {
        sortedAggregatedData.emplace_back(&pair);
    }

    std::sort(sortedAggregatedData.begin(), sortedAggregatedData.end(), [](const TAggregatedData::value_type* lhs,
                                                                           const TAggregatedData::value_type* rhs)
    {
        return lhs->second.AverageInstantiationTime > rhs->second.AverageInstantiationTime;
    });

    // write data header to stream
    out << "Specialization template" << ";"
        << "Primary template" << ";"
        << "Average elapsed time (nanoseconds)" << ";"
        << "Minimum elapsed time (nanoseconds)" << ";"
        << "Maximum elapsed time (nanoseconds)" << ";"
        << "Standard deviation (nanoseconds)" << ";"
        << "Occurrences" << std::endl;

    // write template instantiations
    for (auto&& data : sortedAggregatedData)
    {
        out << data->first << ";"
            << *data->second.PrimaryTemplateKey << ";"
            << data->second.AverageInstantiationTime.count() << ";"
            << data->second.MinimumInstantiationTime.count() << ";"
            << data->second.MaximumInstantiationTime.count() << ";"
            << data->second.StandardDeviation.count() << ";"
            << data->second.InstantiationTimes.size() << std::endl;
    }

    out.close();
    return true;
}
