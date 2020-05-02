#include "FileCompilationsExporter.h"

#include <fstream>
#include <algorithm>

FileCompilationsExporter::FileCompilationsExporter(const TFileCompilationDataPerFile& data)
    : m_data(data)
{
}

FileCompilationsExporter::~FileCompilationsExporter()
{
}

bool FileCompilationsExporter::ExportTo(const std::string& path) const
{
    std::ofstream out(path);
    if (!out)
    {
        return false;
    }

    // create a vector with data
    std::vector<const TFileCompilationDataPerFile::value_type*> dataPerFile;
    for (auto&& pair : m_data)
    {
        dataPerFile.push_back(&pair);
    }

    // sort it (slowest compilations first)
    std::sort(dataPerFile.begin(), dataPerFile.end(), [](const TFileCompilationDataPerFile::value_type* lhs,
                                                         const TFileCompilationDataPerFile::value_type* rhs)
    {
        std::chrono::nanoseconds lhsTotalDuration = lhs->second.BackEnd.Stop - lhs->second.FrontEnd.Start;
        std::chrono::nanoseconds rhsTotalDuration = rhs->second.BackEnd.Stop - rhs->second.FrontEnd.Start;

        return lhsTotalDuration > rhsTotalDuration;
    });

    // write data header to stream
    out << "File path" << ";"
        << "Compilation time (nanoseconds)" << ";"
        << "Front-end time (nanoseconds)" << ";"
        << "Back-end time (nanoseconds)" << std::endl;

    // write data to stream
    for (auto&& data : dataPerFile)
    {
        std::chrono::nanoseconds totalDuration = data->second.BackEnd.Stop - data->second.FrontEnd.Start;

        out << data->first << ";"
            << (data->second.BackEnd.Stop - data->second.FrontEnd.Start).count() << ";"
            << (data->second.FrontEnd.Stop - data->second.FrontEnd.Start).count() << ";"
            << (data->second.BackEnd.Stop - data->second.BackEnd.Start).count() << std::endl;
    }
    
    out.close();
    return true;
}