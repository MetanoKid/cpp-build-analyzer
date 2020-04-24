#include "CppBuildInsightsDataConversion.h"

#include <Windows.h>
#include <DbgHelp.h>
#include <locale>
#include <codecvt>

#include <CppBuildInsights.hpp>
namespace CppBI = Microsoft::Cpp::BuildInsights;

namespace
{
	const DWORD s_undecoratedNameMaxLength = 512;
	const DWORD s_undecorateFlags = UNDNAME_COMPLETE |
									UNDNAME_32_BIT_DECODE |
									UNDNAME_NO_ACCESS_SPECIFIERS;
}

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

		std::string UndecorateFunction(const std::string& functionName)
		{
			char undecoratedFunctionName[s_undecoratedNameMaxLength];
			DWORD result = UnDecorateSymbolName(functionName.c_str(), undecoratedFunctionName,
												s_undecoratedNameMaxLength, s_undecorateFlags);

			return result != 0 ? undecoratedFunctionName : functionName;
		}
	}
}
