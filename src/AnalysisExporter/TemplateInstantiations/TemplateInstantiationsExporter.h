#pragma once

#include <string>

#include "AnalysisData\SymbolNames.h"
#include "AnalysisData\TemplateInstantiationData.h"

class TemplateInstantiationsExporter
{
public:
    TemplateInstantiationsExporter(const TSymbolNames& symbolNames,
                                   const TTemplateInstantiationDataPerOccurrence& templateInstantiations);
    ~TemplateInstantiationsExporter();

    // exports to CSV format
    bool ExportTo(const std::string& path) const;

private:
    struct DataPerTemplate
    {
        const std::string* primaryTemplateKey;
        std::chrono::nanoseconds totalInstantiationTime;
        std::vector<std::chrono::nanoseconds> occurrences;
    };

    const TSymbolNames& m_symbolNames;
    const TTemplateInstantiationDataPerOccurrence& m_templateInstantiations;
};
