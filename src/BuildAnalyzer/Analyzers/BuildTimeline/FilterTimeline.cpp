#include "FilterTimeline.h"

FilterTimeline::FilterTimeline(const Options &options)
    : m_options(options)
{
}

FilterTimeline::~FilterTimeline()
{
}

bool FilterTimeline::Passes(const CppBI::Activities::Function& function) const
{
    return function.Duration() > m_options.IgnoreFunctionsUnder;
}

bool FilterTimeline::Passes(const CppBI::Activities::TemplateInstantiation& templateInstantiation) const
{
    return templateInstantiation.Duration() > m_options.IgnoreTemplatesUnder;
}
