#pragma once

#include <unordered_map>
#include <string>

#include "AnalysisData\FileCompilationData.h"

typedef std::unordered_map<std::string, FileCompilationData> TFileCompilationDataPerFile;
