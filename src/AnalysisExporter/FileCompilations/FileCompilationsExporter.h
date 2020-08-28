#pragma once

#include <string>

#include "AnalysisData\FileCompilationDataPerFile.h"

class FileCompilationsExporter
{
public:
    FileCompilationsExporter(const TFileCompilationDataPerFile& data);
    ~FileCompilationsExporter();

    // exports to CSV
    template<typename T>
    bool ExportTo(const std::string& path) const;

private:
    const TFileCompilationDataPerFile& m_data;
};
