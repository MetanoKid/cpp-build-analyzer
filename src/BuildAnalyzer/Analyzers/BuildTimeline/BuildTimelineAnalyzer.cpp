#include "BuildTimelineAnalyzer.h"

#include <cassert>

#include "AnalysisData\Utilities\CppBuildInsightsDataConversion.h"

BuildTimelineAnalyzer::BuildTimelineAnalyzer()
    : CppBI::IAnalyzer()
    , m_buildTimeline()
    , m_unresolvedTemplateInstantiationsPerFrontEndPass()
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
        CppBI::MatchEventInMemberFunction(eventStack.Back(), this, &BuildTimelineAnalyzer::OnFrontEndPass) ||
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

    bool processedSpecificData =
        CppBI::MatchEventInMemberFunction(eventStack.Back(), this, &BuildTimelineAnalyzer::OnFrontEndPassFinished);

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

void BuildTimelineAnalyzer::OnTemplateInstantiation(const CppBI::Activities::FrontEndPass& frontEndPass,
                                                    const CppBI::Activities::TemplateInstantiation& templateInstantiation)
{
    auto it = m_unresolvedTemplateInstantiationsPerFrontEndPass.find(frontEndPass.EventInstanceId());
    assert(it != m_unresolvedTemplateInstantiationsPerFrontEndPass.end());

    auto result = it->second.try_emplace(templateInstantiation.SpecializationSymbolKey(), TEventInstanceIds());
    result.first->second.push_back(templateInstantiation.EventInstanceId());
}

void BuildTimelineAnalyzer::OnFrontEndPass(const CppBI::Activities::FrontEndPass& frontEndPass)
{
    auto result = m_unresolvedTemplateInstantiationsPerFrontEndPass.try_emplace(frontEndPass.EventInstanceId(), TUnresolvedTemplateInstantiations());
    assert(result.second);
}

void BuildTimelineAnalyzer::OnFrontEndPassFinished(const CppBI::Activities::FrontEndPass& frontEndPass)
{
    auto it = m_unresolvedTemplateInstantiationsPerFrontEndPass.find(frontEndPass.EventInstanceId());
    assert(it != m_unresolvedTemplateInstantiationsPerFrontEndPass.end());

    assert(it->second.empty());
    it->second.clear();
}

// ----------------------------------------------------------------------------

void BuildTimelineAnalyzer::OnSymbolNameEvent(const CppBI::Activities::FrontEndPass& frontEndPass,
                                              const CppBI::SimpleEvents::SymbolName& event)
{
    auto itFrontEndPass = m_unresolvedTemplateInstantiationsPerFrontEndPass.find(frontEndPass.EventInstanceId());
    assert(itFrontEndPass != m_unresolvedTemplateInstantiationsPerFrontEndPass.end());

    auto itUnresolvedTemplateInstantiation = itFrontEndPass->second.find(event.Key());
    if (itUnresolvedTemplateInstantiation != itFrontEndPass->second.end())
    {
        for (auto&& eventInstanceId : itUnresolvedTemplateInstantiation->second)
        {
            m_buildTimeline.UpdateEntryName(eventInstanceId, event.Name());
        }

        itFrontEndPass->second.erase(itUnresolvedTemplateInstantiation);
    }
}

void BuildTimelineAnalyzer::OnCommandLineEvent(const CppBI::Activities::Invocation& invocation,
                                               const CppBI::SimpleEvents::CommandLine& commandLine)
{
    m_buildTimeline.AddEntryProperty(invocation.EventInstanceId(), "Command Line",
                                     Utilities::CppBuildInsightsDataConversion::WideStringToString(commandLine.Value()));
}
