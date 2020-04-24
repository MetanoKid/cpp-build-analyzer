#include "BuildTimelineExporter.h"

#include <fstream>

#include <rapidjson\document.h>
#include <rapidjson\ostreamwrapper.h>
#include <rapidjson\prettywriter.h>

#include "AnalysisData\BuildTimeline\BuildTimeline.h"

BuildTimelineExporter::BuildTimelineExporter(const BuildTimeline& timeline)
	: m_timeline(timeline)
{
}

BuildTimelineExporter::~BuildTimelineExporter()
{
}

bool BuildTimelineExporter::ExportTo(const std::string& path) const
{
	std::ofstream out(path);
	if (!out)
	{
		return false;
	}

	rapidjson::Document document(rapidjson::kObjectType);
	
	// although this is the default time unit representation, make it explicit
	{
		rapidjson::Value displayTimeUnit("ms");
		document.AddMember("displayTimeUnit", displayTimeUnit, document.GetAllocator());
	}

	// add all events
	{
		rapidjson::Value traceEvents(rapidjson::kArrayType);
		for (auto&& root : m_timeline.GetRoots())
		{
			AddEntry(root, traceEvents, document);
		}
		document.AddMember("traceEvents", traceEvents, document.GetAllocator());
	}
	
	// write data to stream
	rapidjson::OStreamWrapper ostreamWrapper(out);
	rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(ostreamWrapper);

	document.Accept(writer);

	out.close();
	return true;
}

void BuildTimelineExporter::AddEntry(const TimelineEntry* entry, rapidjson::Value& traceEvents, rapidjson::Document& document) const
{
	if (entry->GetChildren().size() == 0)
	{
		// leaf entries only write one event
		rapidjson::Value event(rapidjson::kObjectType);

		event.AddMember("ph", "X", document.GetAllocator());
		event.AddMember("pid", static_cast<uint64_t>(entry->GetProcessId()), document.GetAllocator());
		event.AddMember("tid", static_cast<uint64_t>(entry->GetThreadId()), document.GetAllocator());
		event.AddMember("name", entry->GetName(), document.GetAllocator());
		// times in microseconds
		event.AddMember("ts", std::chrono::duration_cast<std::chrono::microseconds>(entry->GetStartTimestamp()).count(), document.GetAllocator());
		event.AddMember("dur", std::chrono::duration_cast<std::chrono::microseconds>(entry->GetFinishTimestamp() - entry->GetStartTimestamp()).count(), document.GetAllocator());

		traceEvents.PushBack(event, document.GetAllocator());
	}
	else
	{
		// write "begin" event
		{
			rapidjson::Value event(rapidjson::kObjectType);

			event.AddMember("ph", "B", document.GetAllocator());
			event.AddMember("pid", static_cast<uint64_t>(entry->GetProcessId()), document.GetAllocator());
			event.AddMember("tid", static_cast<uint64_t>(entry->GetThreadId()), document.GetAllocator());
			event.AddMember("name", entry->GetName(), document.GetAllocator());
			// time in microseconds
			event.AddMember("ts", std::chrono::duration_cast<std::chrono::microseconds>(entry->GetStartTimestamp()).count(), document.GetAllocator());

			traceEvents.PushBack(event, document.GetAllocator());
		}
		
		// write children entries
		for (auto&& child : entry->GetChildren())
		{
			AddEntry(child, traceEvents, document);
		}

		// write "end" event
		{
			rapidjson::Value event(rapidjson::kObjectType);

			event.AddMember("ph", "E", document.GetAllocator());
			event.AddMember("pid", static_cast<uint64_t>(entry->GetProcessId()), document.GetAllocator());
			event.AddMember("tid", static_cast<uint64_t>(entry->GetThreadId()), document.GetAllocator());
			event.AddMember("name", entry->GetName(), document.GetAllocator());
			// time in microseconds
			event.AddMember("ts", std::chrono::duration_cast<std::chrono::microseconds>(entry->GetFinishTimestamp()).count(), document.GetAllocator());

			traceEvents.PushBack(event, document.GetAllocator());
		}
	}
}
