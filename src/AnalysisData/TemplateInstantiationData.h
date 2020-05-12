#pragma once

#include <chrono>
#include <unordered_map>

#include "AnalysisData\SymbolKey.h"
#include "AnalysisData\BuildTimeline\TimelineTypes.h"

struct TemplateInstantiationData
{
    TSymbolKey Primary;
    TSymbolKey Specialization;
    std::chrono::nanoseconds Duration;
};

typedef std::unordered_map<TEventInstanceId, TemplateInstantiationData> TTemplateInstantiationDataPerOccurrence;
