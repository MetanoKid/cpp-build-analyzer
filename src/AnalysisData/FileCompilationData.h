#pragma once

#include <chrono>

struct FileCompilationData
{
    struct Pass
    {
        std::chrono::nanoseconds Start = std::chrono::nanoseconds(0);
        std::chrono::nanoseconds Stop = std::chrono::nanoseconds(0);
    };

    Pass FrontEnd;
    Pass BackEnd;
};
