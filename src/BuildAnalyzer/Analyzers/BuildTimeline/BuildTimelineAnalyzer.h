#pragma once

#include <CppBuildInsights.hpp>
namespace CppBI = Microsoft::Cpp::BuildInsights;

#include "AnalysisData\BuildTimeline\BuildTimeline.h"

class BuildTimelineAnalyzer : public CppBI::IAnalyzer
{
public:
    BuildTimelineAnalyzer();
    virtual ~BuildTimelineAnalyzer();

    CppBI::AnalysisControl OnStartActivity(const CppBI::EventStack& eventStack) override;
    CppBI::AnalysisControl OnStopActivity(const CppBI::EventStack& eventStack) override;
    CppBI::AnalysisControl OnSimpleEvent(const CppBI::EventStack& eventStack) override;

    inline const BuildTimeline& GetTimeline() const { return m_buildTimeline; }

private:
    BuildTimeline m_buildTimeline;
    
    typedef std::vector<TEventInstanceId> TEventInstanceIds;
    typedef std::unordered_map<unsigned long long, TEventInstanceIds> TUnresolvedTemplateInstantiations;
    std::unordered_map<TEventInstanceId, TUnresolvedTemplateInstantiations> m_unresolvedTemplateInstantiationsPerFrontEndPass;

    // generic activity handling
    void OnActivityStartRoot(const CppBI::Activities::Activity& activity);
    void OnActivityStartNested(const CppBI::Activities::Activity& parent,
                               const CppBI::Activities::Activity& activity);
    void OnActivityFinished(const CppBI::Activities::Activity& activity);

    // specific activity handling
    void OnInvocation(const CppBI::Activities::Invocation& invocation);
    void OnFrontEndFile(const CppBI::Activities::FrontEndFile& frontEndFile);
    void OnFunction(const CppBI::Activities::Function& function);
    void OnTemplateInstantiation(const CppBI::Activities::FrontEndPass& frontEndPass,
                                 const CppBI::Activities::TemplateInstantiation& templateInstantiation);
    void OnFrontEndPass(const CppBI::Activities::FrontEndPass& frontEndPass);
    void OnFrontEndPassFinished(const CppBI::Activities::FrontEndPass& frontEndPass);

    // specific event handling
    void OnSymbolNameEvent(const CppBI::Activities::FrontEndPass& frontEndPass,
                           const CppBI::SimpleEvents::SymbolName& event);
    void OnCommandLineEvent(const CppBI::Activities::Activity& parent,
                            const CppBI::SimpleEvents::CommandLine& commandLine);
};
