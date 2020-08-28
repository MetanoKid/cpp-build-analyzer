#include "FileCompilationsExporter.h"

#include <fstream>
#include <algorithm>
#include <chrono>

FileCompilationsExporter::FileCompilationsExporter(const TFileCompilationDataPerFile& data)
    : m_data(data)
{
}

FileCompilationsExporter::~FileCompilationsExporter()
{
}

template<typename T>
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

    std::string timeType = "nanoseconds";

    if (std::is_same<T, std::chrono::seconds>::value)
    {
        timeType = "seconds";
    }
    else if (std::is_same<T, std::chrono::milliseconds>::value)
    {
        timeType = "miliseconds";
    }

    // write data header to stream
    out << "File path" << ";"
        << "Compilation time (" << timeType << ")" << ";"
        << "Front-end time (" << timeType <<  ")" << ";"
        << "Back-end time (" << timeType << ")" << std::endl;

    // write data to stream
    for (auto&& data : dataPerFile)
    {
        const FileCompilationData& perFile = data->second;
        T compileTime = std::chrono::duration_cast<T>(perFile.BackEnd.Stop - perFile.FrontEnd.Start);
        T frontEndTime = std::chrono::duration_cast<T>(perFile.FrontEnd.Stop - perFile.FrontEnd.Start);
        T backEndTime = std::chrono::duration_cast<T>(perFile.BackEnd.Stop - perFile.BackEnd.Start);
        
        out << data->first << ";"
            << compileTime.count() << ";"
            << frontEndTime.count() << ";"
            << backEndTime.count() << std::endl;
    }
    
    out.close();
    return true;
}

template
bool FileCompilationsExporter::ExportTo<std::chrono::seconds>(const std::string& path) const;

template
bool FileCompilationsExporter::ExportTo<std::chrono::milliseconds>(const std::string& path) const;

template
bool FileCompilationsExporter::ExportTo<std::chrono::nanoseconds>(const std::string& path) const;
