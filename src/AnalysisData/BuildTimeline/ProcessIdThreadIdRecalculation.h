#pragma once

#include <utility>
#include <unordered_map>

#include "AnalysisData\BuildTimeline\TimelineTypes.h"

class BuildTimeline;
class TimelineEntry;

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
	typedef TThreadId TThreadIdOffset;
	
	struct EntryWithLocalThreadIdOffsetMaxChildOffset
	{
		const TimelineEntry* entry;
		TThreadIdOffset localThreadIdOffset;

		EntryWithLocalThreadIdOffsetMaxChildOffset(const TimelineEntry* entry,
												   const TThreadIdOffset& localThreadIdOffset)
			: entry(entry)
			, localThreadIdOffset(localThreadIdOffset)
		{
		}
	};
	
	typedef std::unordered_map<TEventInstanceId, EntryWithLocalThreadIdOffsetMaxChildOffset> TThreadOffsetData;
	
private:
	const BuildTimeline& m_timeline;
	std::unordered_map<TEventInstanceId, TProcessThreadPair> m_remappings;

	void CalculateProcessIdRemaps();
	void CalculateThreadIdRemaps();
	void CalculateLocalThreadOffsets(const TimelineEntry* entry, TThreadOffsetData& offsetData) const;
	TThreadIdOffset ApplyThreadIdRemap(const TimelineEntry* entry, const TThreadOffsetData& offsetData,
									   const TProcessId& remappedProcessId, const TThreadId& parentAbsoluteThreadId);
};
