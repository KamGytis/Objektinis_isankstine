#include "wordfrequency.h"
#include "cleanword.h"
#include <fstream>
#include <iostream>
#include <map>
#include <iomanip>

void wordFrequency(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream input(inputFile);
    if (!input.is_open()) { std::cerr << "Klaida: " << inputFile << "\n"; return; }
    std::map<std::string, int> wordCount;
    std::string word;
    while (input >> word) {
        std::string clean = cleanWord(word);
        if (clean.size() > 1) wordCount[clean]++;
    }
    input.close();
    std::ofstream output(outputFile);
    output << "=== ZODZIU DAZNUMAS (pasikartoja > 1 karta) ===\n\n";
    int cnt = 0;
    for (const auto& [z, k] : wordCount) {
        if (k > 1) {
            int len = 0;
            for (size_t i = 0; i < z.size(); ) {
                unsigned char c = z[i];
                if (c < 0x80) i += 1;
                else if (c < 0xE0) i += 2;
                else i += 3;
                len++;
            }
            int padding = 20 - len;
            output << z << std::string(padding > 0 ? padding : 1, ' ') << k << "\n";
            cnt++;
        }
    }
    output << "\n--- Is viso: " << cnt << " ---\n";
    std::cout << "[1] issaugota -> " << outputFile << "\n";
}