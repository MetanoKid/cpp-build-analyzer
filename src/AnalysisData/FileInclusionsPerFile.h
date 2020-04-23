#pragma once

#include <unordered_map>
#include <unordered_set>
#include <string>

typedef std::unordered_set<std::string> TFileInclusions;
typedef std::unordered_map<std::string, TFileInclusions> TFileInclusionsPerFile;
