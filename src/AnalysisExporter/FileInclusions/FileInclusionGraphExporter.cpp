#include "FileInclusionGraphExporter.h"

#include <fstream>

FileInclusionGraphExporter::FileInclusionGraphExporter(const TFileInclusionsPerFile& data)
	: m_data(data)
{
}

FileInclusionGraphExporter::~FileInclusionGraphExporter()
{
}

bool FileInclusionGraphExporter::ExportTo(const std::string& path) const
{
	std::ofstream out(path);
	if (!out)
	{
		return false;
	}

	// instead of using an XML library, we'll output our data directly

	// XML header
	out << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;

	// main element
	out << "<DirectedGraph xmlns=\"http://schemas.microsoft.com/vs/2009/dgml\">" << std::endl;

	// write nodes
	out << "  <Nodes>" << std::endl;
	for (auto&& inclusionsForFile : m_data)
	{
		out << "    <Node Id=\"" << inclusionsForFile.first << "\" />" << std::endl;
	}
	out << "  </Nodes>" << std::endl;

	// write edges
	out << "  <Links>" << std::endl;
	for (auto&& inclusionsForFile : m_data)
	{
		for (auto&& includedFile : inclusionsForFile.second)
		{
			out << "    <Link Source=\"" << inclusionsForFile.first << "\" Target=\"" << includedFile << "\" />" << std::endl;
		}
	}
	out << "  </Links>" << std::endl;

	// closing main element
	out << "</DirectedGraph>" << std::endl;

	out.close();
	return true;
}
