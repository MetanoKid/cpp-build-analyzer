# C++ Build Analyzer

Leverage [C++ Build Insights SDK](https://docs.microsoft.com/cpp/build-insights/reference/sdk/overview) to get interesting data from your MSVC builds.

## Features

  * Generates flame graphs out of build execution:
    ![Flame graph](./readme-samples/flame-graph.png "Flame graph")
    * Also shows template instantiations, if present:
      ![Flame graph: template instantiation](./readme-samples/flame-graph-template-instantiation.png "Flame graph: template instantiation")
  * Generates file inclusion graphs:
    ![File inclusion graph](./readme-samples/file-inclusion-graph.png "File inclusion graph")
  * Lists file compilation times:
    ![File compilation times](./readme-samples/file-compilation-times.png "File compilation times")
  * Lists file inclusion times:
    ![File inclusion times](./readme-samples/file-inclusion-times.png "File inclusion times")
  * Lists function compilation times:
    ![Function compilation times](./readme-samples/function-compilation-times.png "Function compilation times")

### Interactive examples

Previous screenshots were obtained by analyzing a `Rebuild` on `CppBuildAnalyzer` in a `Debug|x64` configuration, within Visual Studio 16.4 (Visual Studio 2019). You can check the files here (they were edited to remove local paths):

  * [Flame graph](./readme-samples/sample-output/BuildTimeline.json): open in Google Chrome trace viewer ([chrome://tracing](chrome://tracing).
  * [File inclusion graph](./readme-samples/sample-output/FileInclusions.dgml): open in Visual Studio.
  * [File compilation times](./readme-samples/sample-output/FileCompilations.csv).
  * [File inclusion times](./readme-samples/sample-output/FileInclusionTimes.csv).
  * [Function compilations](./readme-samples/sample-output/FunctionCompilations.csv).

## Getting started

**Requisites**:

  * Install Visual Studio 2017 Community Edition.
  * Clone and build [vcperf](https://github.com/microsoft/vcperf).
  * Clone and build this repository.

**To collect a trace**:

  * Open an elevated command prompt pointing to vcperf.
  * Execute `vcperf /start SomeSessionName`.
    * If you want full data (including template intantiations) execute `vcperf /start /level3 SomeSessionName` instead.
  * Build your Visual Studio 2017+ project from Visual Studio or command line.
  * Execute `vcperf /stopnoanalyze SomeSessionName TraceFile.etl`.

**To analyze a trace**:

  * Open a command prompt.
  * Execute `CppBuildAnalyzer -i TraceFile.etl --analyze_all`.

## Command line options

**Required**:

    -i, --input arg                        Path to trace file
        --analyze_all                      Perform all analysis the program can do

**Optional**:
    
    -h, --help                             Show help
    
        --analyze_function_compilations    Analyzes function compilations (i.e. how long did each one take)
        --analyze_file_inclusion_times     Analyzes file inclusion times (i.e. how long did each one take)
        --analyze_file_inclusion_graph     Creates a file inclusion graph (i.e. directed graph from include clauses)
        --analyze_file_compilations        Analyzes file compilations (i.e. how long did front-end and back-end take)
        --analyze_build_timeline           Analyzes trace and creates a timeline from it

        --out_function_compilations arg    Path to output function compilations data
        --out_file_inclusion_times arg     Path to output file inclusion times
        --out_file_inclusion_graph arg     Path to output file inclusion graph
        --out_file_compilations arg        Path to output file compilations data
        --out_build_timeline arg           Path to output build timeline

## License

This project is released under [GNU GPLv3](https://github.com/MetanoKid/cpp-build-analyzer/blob/master/LICENSE.md) license.

I started this project thanks to the information I gathered from the community, so I wanted to give something back. You are encouraged to alter it in any way you want, but please continue making it public so the community can benefit from it.

## Acknowledgements

Thanks to Microsoft's C++ Team and [@KevinCadieuxMS](https://twitter.com/KevinCadieuxMS) in particular for the [C++ Build Insights SDK](https://docs.microsoft.com/cpp/build-insights/reference/sdk/overview) and [vcperf](https://github.com/microsoft/vcperf)!
