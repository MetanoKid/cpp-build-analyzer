#pragma once

#include <unordered_map>
#include <vector>

#include "TimelineTypes.h"
#include "TimelineEntry.h"

namespace Microsoft
{
	namespace Cpp
	{
		namespace BuildInsights
		{
			class TraceInfo;

			namespace Activities
			{
				class Activity;
			}
		}
	}
}
namespace CppBI = Microsoft::Cpp::BuildInsights;

class BuildTimeline
{
public:
	BuildTimeline();
	~BuildTimeline();

	void AddRootEntry(const CppBI::Activities::Activity& activity);
	void AddNestedEntry(const CppBI::Activities::Activity& parent,
						const CppBI::Activities::Activity& activity);
	void FinishEntry(const CppBI::Activities::Activity& activity);

private:
	std::unordered_map<TEventInstanceId, TimelineEntry> m_entries;
	std::vector<TimelineEntry*> m_roots;

	TimelineEntry* AddEntry(const CppBI::Activities::Activity& activity);
	TimelineEntry* GetEntry(const TEventInstanceId& id);
};
