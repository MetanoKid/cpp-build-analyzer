#pragma once

#include <chrono>
#include <unordered_map>

#include "AnalysisData\SymbolKey.h"
#include "AnalysisData\BuildTimeline\TimelineTypes.h"

struct TemplateInstantiationData
{
    TSymbolKey Primary = 0;
    TSymbolKey Specialization = 0;
    std::chrono::nanoseconds Duration = std::chrono::nanoseconds(0);
};

typedef std::unordered_map<TEventInstanceId, TemplateInstantiationData> TTemplateInstantiationDataPerOccurrence;
