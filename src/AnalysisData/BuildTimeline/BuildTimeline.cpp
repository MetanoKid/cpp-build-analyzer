#include "BuildTimeline.h"

#include <cassert>
#include <CppBuildInsights.hpp>

#include "..\Utilities\CppBuildInsightsDataConversion.h"

BuildTimeline::BuildTimeline()
{
}

BuildTimeline::~BuildTimeline()
{
}

void BuildTimeline::AddRootEntry(const CppBI::Activities::Activity& activity)
{
	TimelineEntry* entry = AddEntry(activity);

	assert(std::find(m_roots.begin(), m_roots.end(), entry) == m_roots.end());
	m_roots.push_back(entry);
}

void BuildTimeline::AddNestedEntry(const CppBI::Activities::Activity& parent,
								   const CppBI::Activities::Activity& activity)
{
	TimelineEntry* parentEntry = GetEntry(parent.EventInstanceId());
	assert(parentEntry != nullptr);

	TimelineEntry* entry = AddEntry(activity);
	parentEntry->AddChild(entry);
}

TimelineEntry* BuildTimeline::AddEntry(const CppBI::Activities::Activity& activity)
{
	TimelineEntry entry = TimelineEntry(activity.EventInstanceId(),
										activity.EventName(),
										Utilities::CppBuildInsightsDataConversion::Timestamp(activity.StartTimestamp(), activity.TickFrequency()),
										Utilities::CppBuildInsightsDataConversion::Timestamp(activity.StopTimestamp(), activity.TickFrequency()),
										activity.ProcessId(),
										activity.ThreadId(),
										activity.ProcessorIndex());

	auto result = m_entries.try_emplace(activity.EventInstanceId(), entry);
	assert(result.second);

	return &result.first->second;
}

void BuildTimeline::FinishEntry(const CppBI::Activities::Activity& activity)
{
	TimelineEntry* entry = GetEntry(activity.EventInstanceId());
	assert(entry != nullptr);

	entry->SetFinishTimestamp(Utilities::CppBuildInsightsDataConversion::Timestamp(activity.StopTimestamp(), activity.TickFrequency()));
}

void BuildTimeline::UpdateEntryName(const CppBI::Activities::Activity& activity, const std::string& name)
{
	TimelineEntry* entry = GetEntry(activity.EventInstanceId());
	assert(entry != nullptr);

	entry->SetName(name);
}

TimelineEntry* BuildTimeline::GetEntry(const TEventInstanceId& id)
{
	auto itEntry = m_entries.find(id);
	if (itEntry == m_entries.end())
	{
		return nullptr;
	}

	return &itEntry->second;
}
