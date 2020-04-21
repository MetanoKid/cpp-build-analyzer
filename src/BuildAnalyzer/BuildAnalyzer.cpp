#include "BuildAnalyzer.h"

#include <CppBuildInsights.hpp>

namespace
{
	const unsigned int s_numberOfPasses = 1;
}

namespace CppBI = Microsoft::Cpp::BuildInsights;

BuildAnalyzer::BuildAnalyzer()
{
}

BuildAnalyzer::~BuildAnalyzer()
{
}

bool BuildAnalyzer::Analyze(const char* traceFilePath)
{
	auto analyzerGroup = CppBI::MakeStaticAnalyzerGroup(/* TODO: populate */);

	CppBI::RESULT_CODE result = CppBI::Analyze(traceFilePath, s_numberOfPasses, analyzerGroup);
	return result == CppBI::RESULT_CODE::RESULT_CODE_SUCCESS;
}
