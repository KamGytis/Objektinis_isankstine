#pragma once
#include <string>
#include <set>

void loadTLDs(const std::string& filename, std::set<std::string>& tlds);
std::string extractTLD(const std::string& word);
void findURLs(const std::string& inputFile, const std::string& outputFile);