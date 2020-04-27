#pragma once

#include <utility>
#include <unordered_map>

#include "TimelineTypes.h"

class BuildTimeline;

class ProcessIdThreadIdRecalculation
{
public:
	typedef std::pair<TProcessId, TThreadId> TProcessThreadPair;

public:
	ProcessIdThreadIdRecalculation(const BuildTimeline& timeline);
	~ProcessIdThreadIdRecalculation();

	void Calculate();

	const TProcessThreadPair* GetRemapFor(const TEventInstanceId& id) const;

private:
	const BuildTimeline& m_timeline;
	std::unordered_map<TEventInstanceId, TProcessThreadPair> m_remappings;
};
