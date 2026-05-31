#include "urls.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <set>

void loadTLDs(const std::string& filename, std::set<std::string>& tlds) {
    std::ifstream file(filename);
    if (!file.is_open()) { std::cerr << "Klaida: " << filename << "\n"; return; }
    std::string header;
    std::getline(file, header);
    std::string tld;
    while (file >> tld) {
        std::transform(tld.begin(), tld.end(), tld.begin(), ::tolower);
        tlds.insert(tld);
    }
    file.close();
}

std::string extractTLD(const std::string& word) {
    std::string w = word;
    while (!w.empty() && (w.back() == '.' || w.back() == ',' || w.back() == ')' || w.back() == ';'))
        w.pop_back();
    size_t pos = w.rfind('.');
    if (pos == std::string::npos) return "";
    std::string tld = w.substr(pos + 1);
    size_t slash = tld.find('/');
    if (slash != std::string::npos) tld = tld.substr(0, slash);
    return tld;
}

void findURLs(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream input(inputFile);
    if (!input.is_open()) { std::cerr << "Klaida: " << inputFile << "\n"; return; }
    std::set<std::string> tlds;
    loadTLDs("url_end.txt", tlds);
    std::set<std::string> urls;
    std::string word;
    while (input >> word) {
        while (!word.empty() && (word.front() == '(' || word.front() == '"' || word.front() == ')'))
            word.erase(0, 1);
        while (!word.empty() && (word.back() == '.' || word.back() == ',' || word.back() == ')' || word.back() == ';' || word.back() == '"'))
            word.pop_back();
        std::string tld = extractTLD(word);
        if (!tld.empty() && tlds.count(tld) > 0)
            urls.insert(word);
    }
    input.close();
    std::ofstream output(outputFile);
    output << "=== RASTI URL ADRESAI ===\n\n";
    if (urls.empty()) { output << "(URL adresu nerasta)\n"; }
    else { int i = 1; for (const auto& url : urls) output << i++ << ". " << url << "\n"; }
    output << "\n--- Is viso URL: " << urls.size() << " ---\n";
    std::cout << "[3] issaugota -> " << outputFile << "\n";
}