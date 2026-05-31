#include "crossreference.h"
#include "cleanword.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <set>

void crossReference(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream input(inputFile);
    if (!input.is_open()) { std::cerr << "Klaida: " << inputFile << "\n"; return; }
    std::map<std::string, std::set<int>> crossRef;
    std::string line;
    int lineNum = 0;
    while (std::getline(input, line)) {
        lineNum++;
        std::istringstream iss(line);
        std::string word;
        while (iss >> word) {
            std::string clean = cleanWord(word);
            if (!clean.empty()) crossRef[clean].insert(lineNum);
        }
    }
    input.close();
    std::ofstream output(outputFile);
    output << "=== CROSS-REFERENCE LENTELE ===\n\n";
    for (const auto& [zodis, eilutes] : crossRef) {
        if (eilutes.size() > 1) {
            output << zodis << "\n   Eilutes: ";
            for (int el : eilutes) output << el << " ";
            output << "  (skirtingu eiluciu: " << eilutes.size() << ")\n\n";
        }
    }
    std::cout << "[2] issaugota -> " << outputFile << "\n";
}