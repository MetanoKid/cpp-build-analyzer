#pragma once

#include <chrono>

struct FileCompilationData
{
    struct Pass
    {
        std::chrono::nanoseconds Start;
        std::chrono::nanoseconds Stop;
    };

    Pass FrontEnd;
    Pass BackEnd;
};
