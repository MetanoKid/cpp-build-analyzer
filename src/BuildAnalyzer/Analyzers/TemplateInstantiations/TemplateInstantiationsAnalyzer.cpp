#include "TemplateInstantiationsAnalyzer.h"

TemplateInstantiationsAnalyzer::TemplateInstantiationsAnalyzer()
    : CppBI::IAnalyzer()
    , m_symbolNames()
    , m_templateInstantiationData()
{
}

TemplateInstantiationsAnalyzer::~TemplateInstantiationsAnalyzer()
{
}

CppBI::AnalysisControl TemplateInstantiationsAnalyzer::OnStopActivity(const CppBI::EventStack& eventStack)
{
    CppBI::MatchEventInMemberFunction(eventStack.Back(), this, &TemplateInstantiationsAnalyzer::OnTemplateInstantiation);

    return CppBI::AnalysisControl::CONTINUE;
}

CppBI::AnalysisControl TemplateInstantiationsAnalyzer::OnSimpleEvent(const CppBI::EventStack& eventStack)
{
    CppBI::MatchEventInMemberFunction(eventStack.Back(), this, &TemplateInstantiationsAnalyzer::OnSymbolName);

    return CppBI::AnalysisControl::CONTINUE;
}

void TemplateInstantiationsAnalyzer::OnTemplateInstantiation(const CppBI::Activities::TemplateInstantiation& templateInstantiation)
{
    assert(m_templateInstantiationData.find(templateInstantiation.EventInstanceId()) == m_templateInstantiationData.end());

    TemplateInstantiationData data;
    data.Primary = templateInstantiation.PrimaryTemplateSymbolKey();
    data.Specialization = templateInstantiation.SpecializationSymbolKey();
    data.Duration = std::chrono::nanoseconds(templateInstantiation.Duration());

    auto result = m_templateInstantiationData.try_emplace(templateInstantiation.EventInstanceId(), data);
    assert(result.second);
}

void TemplateInstantiationsAnalyzer::OnSymbolName(const CppBI::SimpleEvents::SymbolName& symbolName)
{
    assert(m_symbolNames.find(symbolName.Key()) == m_symbolNames.end());

    auto result = m_symbolNames.try_emplace(symbolName.Key(), symbolName.Name());
    assert(result.second);
}
