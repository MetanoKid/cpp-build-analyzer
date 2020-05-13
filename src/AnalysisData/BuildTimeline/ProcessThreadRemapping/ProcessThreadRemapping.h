#pragma once

#include <unordered_map>

#include "AnalysisData\BuildTimeline\TimelineTypes.h"

struct ProcessThreadRemap
{
    TProcessId ProcessId;
    TThreadId ThreadId;
};

typedef std::unordered_map<TEventInstanceId, ProcessThreadRemap> TProcessThreadRemappings;
