#pragma once

#include <CppBuildInsights.hpp>
namespace CppBI = Microsoft::Cpp::BuildInsights;

#include "AnalysisData\TimeElapsedPerOccurrencePerConcept.h"
#include "AnalysisData\FileInclusionsPerFile.h"

class FileInclusionsAnalyzer : public CppBI::IAnalyzer
{
public:
    FileInclusionsAnalyzer();
    virtual ~FileInclusionsAnalyzer();

    CppBI::AnalysisControl OnStopActivity(const CppBI::EventStack& eventStack) override;

    inline const TTimeElapsedPerOccurrencePerConcept& GetTimeData() const { return m_fileInclusionTimes; }
    inline const TFileInclusionsPerFile& GetFileInclusionsData() const { return m_fileInclusionsPerFile; }

private:
    TTimeElapsedPerOccurrencePerConcept m_fileInclusionTimes;
    TFileInclusionsPerFile m_fileInclusionsPerFile;

    void OnFileParsed(const CppBI::Activities::FrontEndFile& fromFile,
                      const CppBI::Activities::FrontEndFile& includedFile);
};
