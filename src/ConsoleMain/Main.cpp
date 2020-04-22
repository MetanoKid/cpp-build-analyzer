#include <iostream>
#include <string>
#include <cxxopts\cxxopts.hpp>

#include "BuildAnalyzer\BuildAnalyzer.h"

namespace
{
	const std::string s_defaultOutputPathSlowFunctionCompilations = "FunctionCompilations.csv";
}

int main(int argc, char** argv)
{
	cxxopts::Options commandLineOptions = cxxopts::Options(argv[0], "Analyzes C++ builds");

	// variables to populate from options
	std::string inputPathTraceFile;
	std::string outputPathSlowFunctionCompilations = s_defaultOutputPathSlowFunctionCompilations;

	// configure options
	commandLineOptions.add_options()
		("h,help", "Show help")
		("i,input", "Path to trace file", cxxopts::value(inputPathTraceFile))
		("out_slow_function_compilations", "Path to output slow function compilations", cxxopts::value(outputPathSlowFunctionCompilations));

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
		analyzer.ExportFunctionCompilationTimes(outputPathSlowFunctionCompilations);
	}

	std::cout << "Analysis " << (succeeded ? "succeeded" : "failed") << std::endl;

	return 0;
}