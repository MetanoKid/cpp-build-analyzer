#include "BuildTimelineAnalyzer.h"

#include <cassert>

#include "AnalysisData\Utilities\CppBuildInsightsDataConversion.h"

BuildTimelineAnalyzer::BuildTimelineAnalyzer()
    : CppBI::IAnalyzer()
    , m_buildTimeline()
    , m_symbolNames()
    , m_unresolvedTemplateInstantiationsPerSymbol()
{
}

BuildTimelineAnalyzer::~BuildTimelineAnalyzer()
{
}

CppBI::AnalysisControl BuildTimelineAnalyzer::OnStartActivity(const CppBI::EventStack& eventStack)
{
    bool processedBaseData =
        CppBI::MatchEventStackInMemberFunction(eventStack,   this, &BuildTimelineAnalyzer::OnActivityStartNested) ||
        CppBI::MatchEventInMemberFunction(eventStack.Back(), this, &BuildTimelineAnalyzer::OnActivityStartRoot);
    assert(processedBaseData);

    bool processedSpecificData =
        CppBI::MatchEventInMemberFunction(eventStack.Back(), this, &BuildTimelineAnalyzer::OnInvocation) ||
        CppBI::MatchEventInMemberFunction(eventStack.Back(), this, &BuildTimelineAnalyzer::OnFrontEndFile) ||
        CppBI::MatchEventInMemberFunction(eventStack.Back(), this, &BuildTimelineAnalyzer::OnFunction) ||
        CppBI::MatchEventStackInMemberFunction(eventStack,   this, &BuildTimelineAnalyzer::OnTemplateInstantiation);

    return CppBI::AnalysisControl::CONTINUE;
}

CppBI::AnalysisControl BuildTimelineAnalyzer::OnStopActivity(const CppBI::EventStack& eventStack)
{
    bool processed =
        CppBI::MatchEventInMemberFunction(eventStack.Back(), this, &BuildTimelineAnalyzer::OnActivityFinished);
    assert(processed);

    return CppBI::AnalysisControl::CONTINUE;
}

CppBI::AnalysisControl BuildTimelineAnalyzer::OnSimpleEvent(const CppBI::EventStack& eventStack)
{
    bool processed =
        CppBI::MatchEventStackInMemberFunction(eventStack, this, &BuildTimelineAnalyzer::OnSymbolNameEvent) ||
        CppBI::MatchEventStackInMemberFunction(eventStack, this, &BuildTimelineAnalyzer::OnCommandLineEvent);
    
    return CppBI::AnalysisControl::CONTINUE;
}

void BuildTimelineAnalyzer::OnActivityStartRoot(const CppBI::Activities::Activity& activity)
{
    m_buildTimeline.AddRootEntry(activity);
}

void BuildTimelineAnalyzer::OnActivityStartNested(const CppBI::Activities::Activity& parent,
                                                  const CppBI::Activities::Activity& activity)
{
    m_buildTimeline.AddNestedEntry(parent, activity);
}

void BuildTimelineAnalyzer::OnActivityFinished(const CppBI::Activities::Activity& activity)
{
    m_buildTimeline.FinishEntry(activity);
}

// ----------------------------------------------------------------------------

void BuildTimelineAnalyzer::OnInvocation(const CppBI::Activities::Invocation& invocation)
{
    if (invocation.ToolPath() != nullptr)
    {
        m_buildTimeline.AddEntryProperty(invocation.EventInstanceId(), "Tool Path",
                                         Utilities::CppBuildInsightsDataConversion::WideStringToString(invocation.ToolPath()));
    }

    m_buildTimeline.AddEntryProperty(invocation.EventInstanceId(), "Tool Version", invocation.ToolVersionString());
    m_buildTimeline.AddEntryProperty(invocation.EventInstanceId(), "Working Directory",
                                     Utilities::CppBuildInsightsDataConversion::WideStringToString(invocation.WorkingDirectory()));
}

void BuildTimelineAnalyzer::OnFrontEndFile(const CppBI::Activities::FrontEndFile& frontEndFile)
{
    m_buildTimeline.UpdateEntryName(frontEndFile.EventInstanceId(), frontEndFile.Path());
}

void BuildTimelineAnalyzer::OnFunction(const CppBI::Activities::Function& function)
{
    m_buildTimeline.UpdateEntryName(function.EventInstanceId(), Utilities::CppBuildInsightsDataConversion::UndecorateFunction(function.Name()));
}

void BuildTimelineAnalyzer::OnTemplateInstantiation(const CppBI::Activities::TemplateInstantiation& templateInstantiation)
{
    // from the docs: "Type keys are unique within the trace being analyzed. [...]
    //                 SYMBOL_NAME events are emitted at the end of a compiler front-end pass,
    //                 after all template instantiations have taken place."
    assert(m_symbolNames.find(templateInstantiation.SpecializationSymbolKey()) == m_symbolNames.end());
    
    // subscribe (may also have others subscribed already)
    auto result = m_unresolvedTemplateInstantiationsPerSymbol.try_emplace(templateInstantiation.SpecializationSymbolKey(),
                                                                            TUnresolvedTemplateInstantiations());
    result.first->second.push_back(templateInstantiation.EventInstanceId());
}

// ----------------------------------------------------------------------------

void BuildTimelineAnalyzer::OnSymbolNameEvent(const CppBI::SimpleEvents::SymbolName& event)
{
    // from the docs: "Type keys are unique within the trace being analyzed"
    assert(m_symbolNames.find(event.Key()) == m_symbolNames.end());

    auto result = m_symbolNames.try_emplace(event.Key(), event.Name());
    assert(result.second);

    // notify subscribers, if any
    auto itSubscribers = m_unresolvedTemplateInstantiationsPerSymbol.find(event.Key());
    if (itSubscribers != m_unresolvedTemplateInstantiationsPerSymbol.end())
    {
        for (auto&& id : itSubscribers->second)
        {
            m_buildTimeline.UpdateEntryName(id, event.Name());
        }
    }
}

void BuildTimelineAnalyzer::OnCommandLineEvent(const CppBI::Activities::Activity& parent,
                                               const CppBI::SimpleEvents::CommandLine& commandLine)
{
    m_buildTimeline.AddEntryProperty(parent.EventInstanceId(), "Command Line",
                                     Utilities::CppBuildInsightsDataConversion::WideStringToString(commandLine.Value()));
}
