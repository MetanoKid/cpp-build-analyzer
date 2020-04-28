#include "ProcessIdThreadIdRecalculation.h"

#include <cassert>
#include <algorithm>

#include "BuildTimeline.h"

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

	// remap root entries to low-index ProcessId, taking parallel ones into account
	CalculateProcessIdRemaps();

	// remap all other entries to low-index ThreadId within their ProcessId (taken from their root)
	// take special care, as we need to make room for all parallel entries to fit
	// their children within close ThreadId
	CalculateThreadIdRemaps();
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

void ProcessIdThreadIdRecalculation::CalculateProcessIdRemaps()
{
	const std::vector<TimelineEntry*>& roots = m_timeline.GetRoots();
	std::vector<TProcessId> invalidProcessIdRemaps;
	for (int entryIndex = 0; entryIndex < m_timeline.GetRoots().size(); ++entryIndex)
	{
		const TimelineEntry* entry = roots[entryIndex];

		// because entries are sorted by start time, we only need to check their previous siblings for overlaps
		invalidProcessIdRemaps.clear();
		for (int precedingSiblingIndex = entryIndex - 1; precedingSiblingIndex >= 0; --precedingSiblingIndex)
		{
			const TimelineEntry* precedingSibling = roots[precedingSiblingIndex];
			if (entry->OverlapsWith(precedingSibling))
			{
				// preceding sibling has already been processed, so we must have a remap for it
				auto it = m_remappings.find(precedingSibling->GetId());
				assert(it != m_remappings.end());
				
				invalidProcessIdRemaps.push_back(it->second.first);
			}
		}

		// now we've checked for overlaps, find the first free slot
		TProcessId remappedProcessId = 0;
		while (std::find(invalidProcessIdRemaps.begin(), invalidProcessIdRemaps.end(), remappedProcessId) != invalidProcessIdRemaps.end())
		{
			++remappedProcessId;
		}

		// store the remap: because we're a root we start at the lowest ThreadId possible
		m_remappings.try_emplace(entry->GetId(), std::make_pair(remappedProcessId, 0));
	}
}

void ProcessIdThreadIdRecalculation::CalculateThreadIdRemaps()
{
	// our goal is to keep entries as closer together as possible without creating collisions
	// all child entries that execute sequentially can be part of the same TThreadId while parallel ones
	// span several TThreadId
	// the idea is to calculate how many ThreadId each parent need to remap their children close to
	// them, then assign remaps for all entries
	TThreadOffsetData threadOffsets;

	// calculate local offsets from parent ThreadId, store them in the structure
	for (auto&& root : m_timeline.GetRoots())
	{
		CalculateLocalThreadOffsets(root, threadOffsets);
	}

	// apply calculated offsets knowing root's calculated data, and store remappings
	for (auto&& root : m_timeline.GetRoots())
	{
		auto it = m_remappings.find(root->GetId());
		assert(it != m_remappings.end());

		ApplyThreadIdRemap(root, threadOffsets, it->second.first, it->second.second);
	}
}

void ProcessIdThreadIdRecalculation::CalculateLocalThreadOffsets(const TimelineEntry* entry, TThreadOffsetData& offsetData) const
{
	std::vector<TThreadIdOffset> invalidLocalThreadOffsets;
	const std::vector<TimelineEntry*>& children = entry->GetChildren();
	for (int childIndex = 0; childIndex < children.size(); ++childIndex)
	{
		const TimelineEntry* child = children[childIndex];

		// because entries are sorted by start time, children only need to check their previous siblings for overlaps
		invalidLocalThreadOffsets.clear();
		for (int precedingSiblingIndex = childIndex - 1; precedingSiblingIndex >= 0; --precedingSiblingIndex)
		{
			const TimelineEntry* precedingSibling = children[precedingSiblingIndex];
			if (child->OverlapsWith(precedingSibling))
			{
				// preceding sibling has been already processed, we're sure to have a valid remap
				auto it = offsetData.find(precedingSibling->GetId());
				assert(it != offsetData.end());

				invalidLocalThreadOffsets.push_back(it->second.localThreadIdOffset);
			}
		}

		// now we've checked for overlaps, find the first slot we fit
		TThreadIdOffset localOffset = 0;
		while (std::find(invalidLocalThreadOffsets.begin(), invalidLocalThreadOffsets.end(), localOffset) != invalidLocalThreadOffsets.end())
		{
			++localOffset;
		}

		// store it
		auto result = offsetData.try_emplace(child->GetId(), child, localOffset);
		assert(result.second);
	}

	// let children calculate
	for (auto&& child : children)
	{
		if (child->GetChildren().size() > 0)
		{
			CalculateLocalThreadOffsets(child, offsetData);
		}
	}
}

ProcessIdThreadIdRecalculation::TThreadIdOffset ProcessIdThreadIdRecalculation::ApplyThreadIdRemap(const TimelineEntry* entry, const TThreadOffsetData& offsetData,
																								   const TProcessId& remappedProcessId, const TThreadId& parentAbsoluteThreadId)
{
	if (entry->GetChildren().size() == 0)
	{
		return 0;
	}

	// we need to make room for each child that has parallel children, so we can keep them as
	// close as possible in ThreadId terms
	TThreadIdOffset childrenAccumulatedExtraThreadId = 0;

	// sort children by their local thread offset
	std::vector<const TThreadOffsetData::mapped_type*> childrenSortedByLocalThreadIdOffset;
	for (auto&& child : entry->GetChildren())
	{
		auto it = offsetData.find(child->GetId());
		assert(it != offsetData.end());

		childrenSortedByLocalThreadIdOffset.push_back(&it->second);
	}

	std::sort(childrenSortedByLocalThreadIdOffset.begin(), childrenSortedByLocalThreadIdOffset.end(),
		[](const TThreadOffsetData::mapped_type* lhs, const TThreadOffsetData::mapped_type* rhs)
	{
		return lhs->localThreadIdOffset < rhs->localThreadIdOffset;
	});

	assert(childrenSortedByLocalThreadIdOffset.size() > 0);
	assert(childrenSortedByLocalThreadIdOffset.size() == entry->GetChildren().size());

	// consider all children within the same local ThreadId as "sequential" and those from
	// different ThreadId as "parallel"
	TThreadIdOffset currentLocalThreadId = childrenSortedByLocalThreadIdOffset[0]->localThreadIdOffset;
	TThreadIdOffset currentMaxExtraThreadId = 0;
	for (auto&& childData : childrenSortedByLocalThreadIdOffset)
	{
		// when we move to a new ThreadId, accumulate calculated data for previous "sequential" entries
		if (currentLocalThreadId != childData->localThreadIdOffset)
		{
			childrenAccumulatedExtraThreadId += currentMaxExtraThreadId;
		}

		// this child's ThreadId starts from the parent's, with its local offset and the offset from previous entries
		TThreadIdOffset remappedThreadId = parentAbsoluteThreadId +
										   childData->localThreadIdOffset +
										   childrenAccumulatedExtraThreadId;

		// let child calculate how much room we need to make to fit its hierarchy, and apply remaps in the process
		TThreadIdOffset childrenHighestExtraThreadId = ApplyThreadIdRemap(childData->entry, offsetData,
																		  remappedProcessId, remappedThreadId);

		// sequential entries fit in the same ThreadId, only need to keep the one that needs more room for its hierarchy
		if (currentLocalThreadId == childData->localThreadIdOffset)
		{
			if (childrenHighestExtraThreadId > currentMaxExtraThreadId)
			{
				currentMaxExtraThreadId = childrenHighestExtraThreadId;
			}
		}
		// when we move to a different ThreadId, prepare next iterations
		else
		{
			currentLocalThreadId = childData->localThreadIdOffset;
			currentMaxExtraThreadId = childrenHighestExtraThreadId;
		}

		// store remap
		auto result = m_remappings.try_emplace(childData->entry->GetId(), std::make_pair(remappedProcessId, remappedThreadId));
		assert(result.second);
	}

	// last sequence of entries weren't accumulated: we didn't find an entry in the "next" ThreadId
	// so, take it into account as well
	childrenAccumulatedExtraThreadId += currentMaxExtraThreadId;

	// tell parent this is the total room we need to fit our hierarchy	
	return currentLocalThreadId + childrenAccumulatedExtraThreadId;
}
