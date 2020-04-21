#include "BuildAnalyzer.h"

#include <CppBuildInsights.hpp>

#include "Analyzers\SlowFunctionCompilationAnalyzer.h"

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

bool BuildAnalyzer::Analyze(const std::string& traceFilePath)
{
	SlowFunctionCompilationAnalyzer slowFunctionCompilationAnalyzer;

	auto analyzerGroup = CppBI::MakeStaticAnalyzerGroup(&slowFunctionCompilationAnalyzer);

	CppBI::RESULT_CODE result = CppBI::Analyze(traceFilePath.c_str(), s_numberOfPasses, analyzerGroup);
	return result == CppBI::RESULT_CODE::RESULT_CODE_SUCCESS;
}
