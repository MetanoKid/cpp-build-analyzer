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
	bool analyzeAll = false;
	bool analyzeFunctionCompilations = false;
	bool analyzeFileInclusionTimes = false;
	bool createFileInclusionGraph = false;
	bool analyzeFileCompilations = false;
	bool createBuildTimeline = false;
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
		// what to analyze
		("analyze_all", "Perform all analysis the program can do", cxxopts::value(analyzeAll))
		("analyze_function_compilations", "Analyzes function compilations (i.e. how long did each one take)", cxxopts::value(analyzeFunctionCompilations))
		("analyze_file_inclusion_times", "Analyzes file inclusion times (i.e. how long did each one take)", cxxopts::value(analyzeFileInclusionTimes))
		("analyze_file_inclusion_graph", "Creates a file inclusion graph (i.e. directed graph from include clauses)", cxxopts::value(createFileInclusionGraph))
		("analyze_file_compilations", "Analyzes file compilations (i.e. how long did front-end and back-end take)", cxxopts::value(analyzeFileCompilations))
		("analyze_build_timeline", "Analyzes trace and creates a timeline from it", cxxopts::value(createBuildTimeline))
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
		std::cout << "Missing input file option (try \"-i TraceFile.etl\")" << std::endl;
		exit(-1);
	}

	// requres something to build
	if (!analyzeAll &&
		!analyzeFunctionCompilations &&
		!analyzeFileInclusionTimes &&
		!createFileInclusionGraph &&
		!analyzeFileCompilations &&
		!createBuildTimeline)
	{
		std::cout << "Missing analysis flags (try \"--analyze_all\")" << std::endl;
		exit(-1);
	}

	// what to build
	BuildAnalyzer::AnalysisOptions analysisOptions;
	analysisOptions.functionCompilations = analyzeAll || analyzeFunctionCompilations;
	analysisOptions.fileInclusionTimes = analyzeAll || analyzeFileInclusionTimes;
	analysisOptions.fileInclusionGraph = analyzeAll || createFileInclusionGraph;
	analysisOptions.fileCompilations = analyzeAll || analyzeFileCompilations;
	analysisOptions.buildTimeline = analyzeAll || createBuildTimeline;

	// analyze trace
	std::cout << "Analyzing..." << std::endl;
	BuildAnalyzer analyzer(inputPathTraceFile, analysisOptions);
	bool succeeded = analyzer.Analyze();
	
	// export data
	if (succeeded)
	{
		if (analysisOptions.functionCompilations)
		{
			std::cout << "Exporting function compilations..." << std::endl;
			
			if (!outputPathFunctionCompilations.empty())
			{
				analyzer.ExportFunctionCompilationsData(outputPathFunctionCompilations);
			}
			else
			{
				std::cout << "  Output path can't be empty!" << std::endl;
			}
		}

		if (analysisOptions.fileInclusionTimes)
		{
			std::cout << "Exporting file inclusion times..." << std::endl;

			if (!outputPathFileInclusionTimes.empty())
			{
				analyzer.ExportFileInclusionTimesData(outputPathFileInclusionTimes);
			}
			else
			{
				std::cout << "  Output path can't be empty!" << std::endl;
			}
		}
		
		if (analysisOptions.fileInclusionGraph)
		{
			std::cout << "Exporting file inclusion graph..." << std::endl;

			if (!outputPathFileInclusionGraph.empty())
			{
				analyzer.ExportFileInclusionGraph(outputPathFileInclusionGraph);
			}
			else
			{
				std::cout << "  Output path can't be empty!" << std::endl;
			}
		}
		
		if (analysisOptions.fileCompilations)
		{
			std::cout << "Exporting file compilations..." << std::endl;

			if (!outputPathFileCompilations.empty())
			{
				analyzer.ExportFileCompilationsData(outputPathFileCompilations);
			}
			else
			{
				std::cout << "  Output path can't be empty!" << std::endl;
			}
		}
		
		if (analysisOptions.buildTimeline)
		{
			std::cout << "Exporting build timeline..." << std::endl;
			
			if (!outputPathBuildTimeline.empty())
			{
				analyzer.ExportBuildTimeline(outputPathBuildTimeline);
			}
			else
			{
				std::cout << "  Output path can't be empty!" << std::endl;
			}
		}
	}

	std::cout << "Analysis " << (succeeded ? "succeeded" : "failed") << std::endl;
	return succeeded ? EXIT_SUCCESS : EXIT_FAILURE;
}