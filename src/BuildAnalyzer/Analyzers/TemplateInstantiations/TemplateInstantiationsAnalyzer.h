#pragma once

#include <unordered_map>
#include <string>

#include <CppBuildInsights.hpp>
namespace CppBI = Microsoft::Cpp::BuildInsights;

#include "AnalysisData\BuildTimeline\TimelineTypes.h"
#include "AnalysisData\TemplateInstantiationData.h"

class TemplateInstantiationsAnalyzer : public CppBI::IAnalyzer
{
public:
    TemplateInstantiationsAnalyzer();
    virtual ~TemplateInstantiationsAnalyzer();

    CppBI::AnalysisControl OnStartActivity(const CppBI::EventStack& eventStack) override;
    CppBI::AnalysisControl OnSimpleEvent(const CppBI::EventStack& eventStack) override;

private:
    typedef unsigned long long TSymbolKey;
    std::unordered_map<TSymbolKey, std::string> m_symbolNames;

    std::unordered_map<TEventInstanceId, TemplateInstantiationData> m_templateInstantiationData;

    void OnTemplateInstantiation(const CppBI::Activities::TemplateInstantiation& templateInstantiation);
    void OnSymbolName(const CppBI::SimpleEvents::SymbolName& symbolName);
};
