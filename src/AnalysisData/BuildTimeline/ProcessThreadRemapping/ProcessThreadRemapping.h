#pragma once

#include <unordered_map>

#include "AnalysisData\BuildTimeline\TimelineTypes.h"

struct ProcessThreadRemap
{
    TProcessId ProcessId = 0;
    TThreadId ThreadId = 0;
};

typedef std::unordered_map<TEventInstanceId, ProcessThreadRemap> TProcessThreadRemappings;
