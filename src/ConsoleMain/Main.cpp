#include <iostream>
#include <cxxopts\cxxopts.hpp>

#include "BuildAnalyzer\BuildAnalyzer.h"

int main(int argc, char** argv)
{
	cxxopts::Options commandLineOptions = cxxopts::Options(argv[0], "Analyzes C++ builds");

	// list options
	commandLineOptions.add_options()
		("h,help", "Show help")
		("i,input", "Path to trace file", cxxopts::value<std::string>());

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
	BuildAnalyzer analyzer;
	bool succeeded = analyzer.Analyze(result["input"].as<std::string>());

	std::cout << "Analysis " << (succeeded ? "succeeded" : "failed") << std::endl;

	return 0;
}