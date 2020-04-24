#pragma once

#include <chrono>
#include <string>
#include <vector>

#include "TimelineTypes.h"

class TimelineEntry
{
public:
	TimelineEntry(const TEventInstanceId& id,
				  const char* name,
				  std::chrono::nanoseconds startTimestamp,
				  std::chrono::nanoseconds finishTimestamp,
				  const TProcessId& processId,
				  const TThreadId& threadId,
				  const TProcessorIndex processorIndex);
	~TimelineEntry();

	void AddChild(TimelineEntry* entry);
	void SetParent(TimelineEntry* entry);

	void SetFinishTimestamp(const std::chrono::nanoseconds& timestamp);

private:
	TEventInstanceId m_id;
	std::string m_name;
	std::chrono::nanoseconds m_startTimestamp;
	std::chrono::nanoseconds m_finishTimestamp;
	TProcessId m_processId;
	TThreadId m_threadId;
	TProcessorIndex m_processorIndex;

	TimelineEntry* m_parent;
	std::vector<TimelineEntry*> m_children;
};
