#pragma once

#include <string>

class SlowFunctionCompilationsExporter
{
public:
	SlowFunctionCompilationsExporter();
	~SlowFunctionCompilationsExporter();

	bool ExportTo(const std::string& path) const;
};
