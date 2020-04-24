#include "CppBuildInsightsDataConversion.h"

#include <locale>
#include <codecvt>

#include <CppBuildInsights.hpp>
namespace CppBI = Microsoft::Cpp::BuildInsights;

namespace Utilities
{
	namespace CppBuildInsightsDataConversion
	{
		std::string FilePath(const wchar_t* filePath)
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
			return converter.to_bytes(filePath);
		}

		std::chrono::nanoseconds Timestamp(long long timestamp, long long fromTickFrequency)
		{
			long long convertedTicks = CppBI::Internal::ConvertTickPrecision(timestamp,
																			 fromTickFrequency,
																			 std::chrono::nanoseconds::period::den);
			return std::chrono::nanoseconds(convertedTicks);
		}
	}
}
