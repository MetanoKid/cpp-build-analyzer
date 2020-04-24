#pragma once

#include <string>
#include <chrono>

namespace Utilities
{
	namespace CppBuildInsightsDataConversion
	{
		std::string FilePath(const wchar_t* filePath);
		std::chrono::nanoseconds Timestamp(long long timestamp, long long fromTickFrequency);
		std::string UndecorateFunction(const std::string& functionName);
	}
}
