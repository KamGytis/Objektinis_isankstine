#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <cctype>

std::string cleanWord(const std::string& word) {
    std::string result;
    size_t i = 0;
    while (i < word.size()) {
        unsigned char c = (unsigned char)word[i];
        if (c < 0x80) {
            if (std::isalpha(c)) result += (char)std::tolower(c);
            else if (c == '-' && !result.empty()) result += '-';
            i++;
        }
        else {
            int bytes = 1;
            if ((c & 0xE0) == 0xC0) bytes = 2;
            else if ((c & 0xF0) == 0xE0) bytes = 3;
            else if ((c & 0xF8) == 0xF0) bytes = 4;
            for (int b = 0; b < bytes && i + (size_t)b < word.size(); b++)
                result += word[i + b];
            i += bytes;
        }
    }
    while (!result.empty() && result.back() == '-') result.pop_back();
    return result;
}

void wordFrequency(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream input(inputFile);
    if (!input.is_open()) { std::cerr << "Klaida: " << inputFile << "\n"; return; }
    std::map<std::string, int> wordCount;
    std::string word;
    while (input >> word) {
        std::string clean = cleanWord(word);
        if (!clean.empty()) wordCount[clean]++;
    }
    input.close();
    std::ofstream output(outputFile);
    output << "=== ZODZIU DAZNUMAS (pasikartoja > 1 karta) ===\n\n";
    int cnt = 0;
    for (const auto& [z, k] : wordCount)
        if (k > 1) { output << z << " : " << k << "\n"; cnt++; }
    output << "\n--- Is viso: " << cnt << " ---\n";
    std::cout << "[1] issaugota -> " << outputFile << "\n";
}

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
        while (!word.empty() && (word.back() == '.' || word.back() == ',' || word.back() == ')' || word.back() == ';'))
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

int main() {
    const std::string INPUT = "tekstas.txt";
    std::cout << "Asociatyviu konteineriu uzduotis\n\n";
    wordFrequency(INPUT, "1_zodziu_daznumas.txt");
    crossReference(INPUT, "2_crossreference.txt");
    findURLs(INPUT, "3_urls.txt");
    std::cout << "\nVisi rezultatai issaugoti!\n";
    return 0;
}