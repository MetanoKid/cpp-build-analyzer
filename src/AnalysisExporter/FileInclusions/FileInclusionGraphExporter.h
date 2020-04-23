#pragma once

#include <string>

#include "AnalysisData\FileInclusionsPerFile.h"

class FileInclusionGraphExporter
{
public:
	FileInclusionGraphExporter(const TFileInclusionsPerFile& data);
	~FileInclusionGraphExporter();

	bool ExportTo(const std::string& path) const;

private:
	const TFileInclusionsPerFile& m_data;
};
