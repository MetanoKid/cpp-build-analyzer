#pragma once

#include <string>

// forward declare rapidjson::Value and rapidjson::Document
namespace rapidjson
{
	class CrtAllocator;

	template <typename BaseAllocator>
	class MemoryPoolAllocator;

	template <typename Encoding, typename Allocator, typename StackAllocator>
	class GenericDocument;

	template<typename CharType>
	struct UTF8;

	typedef GenericDocument<UTF8<char>, MemoryPoolAllocator<CrtAllocator>, CrtAllocator> Document;

	template <typename Encoding, typename Allocator>
	class GenericValue;

	typedef GenericValue<UTF8<char>, MemoryPoolAllocator<CrtAllocator>> Value;
}

class BuildTimeline;
class TimelineEntry;

class BuildTimelineExporter
{
public:
	BuildTimelineExporter(const BuildTimeline& timeline);
	~BuildTimelineExporter();

	// exports to Google Chrome trace format
	bool ExportTo(const std::string& path) const;

private:
	const BuildTimeline& m_timeline;

	void AddEntry(const TimelineEntry* entry, rapidjson::Value& traceEvents, rapidjson::Document& document) const;
};
