#include "ProcessIdThreadIdRecalculation.h"

#include <cassert>

ProcessIdThreadIdRecalculation::ProcessIdThreadIdRecalculation(const BuildTimeline& timeline)
	: m_timeline(timeline)
	, m_remappings()
{
}

ProcessIdThreadIdRecalculation::~ProcessIdThreadIdRecalculation()
{
}

void ProcessIdThreadIdRecalculation::Calculate()
{
	assert(m_remappings.empty());
}

const ProcessIdThreadIdRecalculation::TProcessThreadPair* ProcessIdThreadIdRecalculation::GetRemapFor(const TEventInstanceId& id) const
{
	auto it = m_remappings.find(id);
	if (it == m_remappings.end())
	{
		return nullptr;
	}

	return &it->second;
}
