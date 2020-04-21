#pragma once

#include <string>

class BuildAnalyzer
{
public:
	BuildAnalyzer();
	~BuildAnalyzer();

	bool Analyze(const char* traceFilePath);

private:
};
