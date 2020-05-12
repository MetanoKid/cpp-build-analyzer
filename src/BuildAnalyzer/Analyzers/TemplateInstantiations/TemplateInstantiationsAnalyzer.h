#pragma once

#include <unordered_map>
#include <string>

#include <CppBuildInsights.hpp>
namespace CppBI = Microsoft::Cpp::BuildInsights;

#include "AnalysisData\BuildTimeline\TimelineTypes.h"
#include "AnalysisData\TemplateInstantiationData.h"
#include "AnalysisData\SymbolNames.h"

class TemplateInstantiationsAnalyzer : public CppBI::IAnalyzer
{
public:
    TemplateInstantiationsAnalyzer();
    virtual ~TemplateInstantiationsAnalyzer();

    CppBI::AnalysisControl OnStopActivity(const CppBI::EventStack& eventStack) override;
    CppBI::AnalysisControl OnSimpleEvent(const CppBI::EventStack& eventStack) override;

    inline const TSymbolNames& GetSymbolNames() const { return m_symbolNames; }
    inline const TTemplateInstantiationDataPerOccurrence& GetTemplateInstantiations() const { return m_templateInstantiationData; }

private:
    TSymbolNames m_symbolNames;
    TTemplateInstantiationDataPerOccurrence m_templateInstantiationData;

    void OnTemplateInstantiation(const CppBI::Activities::TemplateInstantiation& templateInstantiation);
    void OnSymbolName(const CppBI::SimpleEvents::SymbolName& symbolName);
};
