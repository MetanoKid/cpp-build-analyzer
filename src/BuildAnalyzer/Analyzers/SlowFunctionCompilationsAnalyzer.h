#pragma once

#include <unordered_map>
#include <vector>
#include <chrono>

#include <CppBuildInsights.hpp>
namespace CppBI = Microsoft::Cpp::BuildInsights;

class SlowFunctionCompilationsAnalyzer : public CppBI::IAnalyzer
{
public:
	SlowFunctionCompilationsAnalyzer();
	virtual ~SlowFunctionCompilationsAnalyzer();

	CppBI::AnalysisControl OnStopActivity(const CppBI::EventStack& eventStack) override;

private:
	typedef std::chrono::nanoseconds TDuration;
	typedef std::vector<TDuration> TDurations;
	typedef std::unordered_map<std::string, TDurations> TFunctionDurations;

	TFunctionDurations m_functionDurations;

	void OnFunction(const CppBI::Activities::Function& function);
};
