#include <iostream>
#include <cxxopts\cxxopts.hpp>

int main(int argc, char** argv)
{
	cxxopts::Options commandLineOptions = cxxopts::Options(argv[0], "Analyzes C++ builds");

	// list options
	commandLineOptions.add_options()
		("h,help", "Show help")
		("in", "Path to trace", cxxopts::value<std::string>());

	// parse command line
	cxxopts::ParseResult result = commandLineOptions.parse(argc, argv);
	
	// display help
	if (result.count("h") > 0)
	{
		std::cout << commandLineOptions.help() << std::endl;
		exit(0);
	}

	// TODO: analyze trace

	return 0;
}