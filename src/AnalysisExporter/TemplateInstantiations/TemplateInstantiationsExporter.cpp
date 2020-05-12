#include "TemplateInstantiationsExporter.h"

#include <fstream>
#include <algorithm>
#include <cassert>

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
        auto itSpecializationTemplateName = m_symbolNames.find(pair.second.Specialization);
        assert(itSpecializationTemplateName != m_symbolNames.end());

        // from the docs: "Comparing types between different compiler front-end passes requires using symbol names"
        auto result = aggregatedData.try_emplace(itSpecializationTemplateName->second, DataPerTemplate());
        if (result.second)
        {
            // only need to point to one of them, as they should be using the same string
            auto itPrimaryTemplateName = m_symbolNames.find(pair.second.Primary);
            assert(itPrimaryTemplateName != m_symbolNames.end());

            result.first->second.primaryTemplateKey = &itPrimaryTemplateName->second;
        }

        result.first->second.occurrences.push_back(pair.second.Duration);
        result.first->second.totalInstantiationTime += pair.second.Duration;
    }

    // sort
    typedef std::pair<const std::string*, const DataPerTemplate*> DataPerSpecialization;
    std::vector<const TAggregatedData::value_type*> sortedAggregatedData;
    for (auto&& pair : aggregatedData)
    {
        sortedAggregatedData.emplace_back(&pair);
    }

    std::sort(sortedAggregatedData.begin(), sortedAggregatedData.end(), [](const TAggregatedData::value_type* lhs,
                                                                           const TAggregatedData::value_type* rhs)
    {
        return lhs->second.totalInstantiationTime > rhs->second.totalInstantiationTime;
    });

    // write data header to stream
    out << "Specialization template" << ";"
        << "Primary template" << ";"
        << "Total elapsed time (nanoseconds)" << ";"
        << "Average elapsed time (nanoseconds)" << ";"
        << "Occurrences" << std::endl;

    // write template instantiations
    for (auto&& data : sortedAggregatedData)
    {
        out << data->first << ";"
            << *data->second.primaryTemplateKey << ";"
            << data->second.totalInstantiationTime.count() << ";"
            << (data->second.totalInstantiationTime / data->second.occurrences.size()).count() << ";"
            << data->second.occurrences.size() << std::endl;
    }

    out.close();
    return true;
}
