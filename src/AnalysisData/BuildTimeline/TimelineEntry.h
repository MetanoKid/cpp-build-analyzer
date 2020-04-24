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

	void SetName(const std::string& name);
	void SetFinishTimestamp(const std::chrono::nanoseconds& timestamp);

	inline const std::vector<TimelineEntry*>& GetChildren() const { return m_children; }
	inline const std::string& GetName() const { return m_name; }
	inline const std::chrono::nanoseconds& GetStartTimestamp() const { return m_startTimestamp; }
	inline const std::chrono::nanoseconds& GetFinishTimestamp() const { return m_finishTimestamp; }
	inline const TProcessId& GetProcessId() const { return m_processId; }
	inline const TThreadId& GetThreadId() const { return m_threadId; }

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
