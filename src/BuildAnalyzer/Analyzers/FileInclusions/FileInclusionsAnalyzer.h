#pragma once

#include <CppBuildInsights.hpp>
namespace CppBI = Microsoft::Cpp::BuildInsights;

#include "AnalysisData\TimeElapsedPerOccurrencePerConcept.h"

class FileInclusionsAnalyzer : public CppBI::IAnalyzer
{
public:
	FileInclusionsAnalyzer();
	virtual ~FileInclusionsAnalyzer();

	CppBI::AnalysisControl OnStopActivity(const CppBI::EventStack& eventStack) override;

	inline const TTimeElapsedPerOccurrencePerConcept& GetData() const { return m_fileInclusionsData; }

private:
	TTimeElapsedPerOccurrencePerConcept m_fileInclusionsData;

	void OnFileParsed(const CppBI::Activities::FrontEndFile& fromFile,
					  const CppBI::Activities::FrontEndFile& includedFile);
};
