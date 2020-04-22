#pragma once

#include <CppBuildInsights.hpp>
namespace CppBI = Microsoft::Cpp::BuildInsights;

#include "AnalysisData\TimeElapsedPerOccurrencePerConcept.h"

class FileInclusionTimeAnalyzer : public CppBI::IAnalyzer
{
public:
	FileInclusionTimeAnalyzer();
	virtual ~FileInclusionTimeAnalyzer();

	CppBI::AnalysisControl OnStopActivity(const CppBI::EventStack& eventStack) override;

	inline const TTimeElapsedPerOcurrencePerConcept& GetInclusionDurations() const { return m_inclusionDurations; }

private:
	TTimeElapsedPerOcurrencePerConcept m_inclusionDurations;

	void OnFileParsed(const CppBI::Activities::FrontEndFile& fromFile,
					  const CppBI::Activities::FrontEndFile& includedFile);
};
