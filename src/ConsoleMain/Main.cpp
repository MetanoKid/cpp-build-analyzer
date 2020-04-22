#include <iostream>
#include <string>
#include <cxxopts\cxxopts.hpp>

#include "BuildAnalyzer\BuildAnalyzer.h"

namespace
{
	const std::string s_defaultOutputPathFunctionCompilations = "FunctionCompilations.csv";
	const std::string s_defaultOutputPathFileInclusions = "FileInclusions.csv";
}

int main(int argc, char** argv)
{
	cxxopts::Options commandLineOptions = cxxopts::Options(argv[0], "Analyzes C++ builds");

	// variables to populate from options
	std::string inputPathTraceFile;
	std::string outputPathFunctionCompilations = s_defaultOutputPathFunctionCompilations;
	std::string outputPathFileInclusions = s_defaultOutputPathFileInclusions;

	// configure options
	commandLineOptions.add_options()
		("h,help", "Show help")
		("i,input", "Path to trace file", cxxopts::value(inputPathTraceFile))
		("out_function_compilations", "Path to output function compilations data", cxxopts::value(outputPathFunctionCompilations))
		("out_file_inclusions", "Path to output file inclusions data", cxxopts::value(outputPathFileInclusions));

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
		analyzer.ExportFileInclusionsData(outputPathFileInclusions);
	}

	std::cout << "Analysis " << (succeeded ? "succeeded" : "failed") << std::endl;

	return 0;
}