#pragma once

#include <string>
#include <chrono>

namespace Utilities
{
    namespace CppBuildInsightsDataConversion
    {
        std::string WideStringToString(const wchar_t* wchar);
        std::chrono::nanoseconds Timestamp(long long timestamp, long long fromTickFrequency);
        std::string UndecorateFunction(const std::string& functionName);
    }
}
