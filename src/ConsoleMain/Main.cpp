#include <iostream>
#include <string>
#include <cxxopts\cxxopts.hpp>

#include "BuildAnalyzer\BuildAnalyzer.h"

namespace
{
	const std::string s_defaultOutputPathFunctionCompilations = "FunctionCompilations.csv";
	const std::string s_defaultoutputPathFileInclusionTimes = "FileInclusionTimes.csv";
	const std::string s_defaultOutputPathFileInclusionGraph = "FileInclusions.dgml";
	const std::string s_defaultOutputPathFileCompilations = "FileCompilations.csv";
	const std::string s_defaultOutputPathBuildTimeline = "BuildTimeline.json";
}

int main(int argc, char** argv)
{
	cxxopts::Options commandLineOptions = cxxopts::Options(argv[0], "Analyzes C++ builds");

	// variables to populate from options
	std::string inputPathTraceFile;
	std::string outputPathFunctionCompilations = s_defaultOutputPathFunctionCompilations;
	std::string outputPathFileInclusionTimes = s_defaultoutputPathFileInclusionTimes;
	std::string outputPathFileInclusionGraph = s_defaultOutputPathFileInclusionGraph;
	std::string outputPathFileCompilations = s_defaultOutputPathFileCompilations;
	std::string outputPathBuildTimeline = s_defaultOutputPathBuildTimeline;

	// configure options
	commandLineOptions.add_options()
		// help
		("h,help", "Show help")
		// input
		("i,input", "Path to trace file", cxxopts::value(inputPathTraceFile))
		// outputs
		("out_function_compilations", "Path to output function compilations data", cxxopts::value(outputPathFunctionCompilations))
		("out_file_inclusion_times", "Path to output file inclusion times", cxxopts::value(outputPathFileInclusionTimes))
		("out_file_inclusion_graph", "Path to output file inclusion graph", cxxopts::value(outputPathFileInclusionGraph))
		("out_file_compilations", "Path to output file compilations data", cxxopts::value(outputPathFileCompilations))
		("out_build_timeline", "Path to output build timeline", cxxopts::value(outputPathBuildTimeline));

	// parse command line
	cxxopts::ParseResult result = commandLineOptions.parse(argc, argv);
	
	// display help
	if (result.count("help") > 0)
	{
		std::cout << commandLineOptions.help() << std::endl;
		exit(0);
	}

	// requires input file
	if (result.count("input") == 0)
	{
		std::cout << "Missing input file option" << std::endl;
		exit(-1);
	}

	// analyze trace
	BuildAnalyzer analyzer(inputPathTraceFile);
	bool succeeded = analyzer.Analyze();
	
	// export data
	if (succeeded)
	{
		analyzer.ExportFunctionCompilationsData(outputPathFunctionCompilations);
		analyzer.ExportFileInclusionTimesData(outputPathFileInclusionTimes);
		analyzer.ExportFileInclusionGraph(outputPathFileInclusionGraph);
		analyzer.ExportFileCompilationsData(outputPathFileCompilations);
		analyzer.ExportBuildTimeline(outputPathBuildTimeline);
	}

	std::cout << "Analysis " << (succeeded ? "succeeded" : "failed") << std::endl;

	return 0;
}