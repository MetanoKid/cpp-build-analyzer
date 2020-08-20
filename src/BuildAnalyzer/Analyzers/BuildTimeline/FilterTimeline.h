#pragma once

#include <chrono>

#include <CppBuildInsights.hpp>
namespace CppBI = Microsoft::Cpp::BuildInsights;

class FilterTimeline
{
public:
    struct Options
    {
        std::chrono::nanoseconds IgnoreFunctionsUnder = std::chrono::nanoseconds(0);
        std::chrono::nanoseconds IgnoreTemplatesUnder = std::chrono::nanoseconds(0);
    };

public:
    FilterTimeline(const Options& options);
    ~FilterTimeline();

    bool Passes(const CppBI::Activities::Function& function) const;
    bool Passes(const CppBI::Activities::TemplateInstantiation& templateInstantiation) const;

private:
    Options m_options;
};
