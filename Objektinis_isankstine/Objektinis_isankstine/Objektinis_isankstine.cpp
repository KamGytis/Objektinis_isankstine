#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <cctype>
#include <regex>

//  Pagalbinė funkcija – žodžio valymas
//  Palaiko UTF-8: ASCII raides keičia į mažąsias,
//  lietuviškus daugiabaičius simbolius (š,ž,ė...) praleidžia nepakeistu.
std::string cleanWord(const std::string& word) {
    std::string result;
    size_t i = 0;
    while (i < word.size()) {
        unsigned char c = (unsigned char)word[i];

        if (c < 0x80) {
            // ASCII simbolis
            if (std::isalpha(c))
                result += (char)std::tolower(c);
            else if (c == '-' && !result.empty())
                result += '-';
            i++;
        }
        else {
            // Daugiabaitis UTF-8 simbolis (lietuviškos raidės ir pan.)
            int bytes = 1;
            if ((c & 0xE0) == 0xC0) bytes = 2;
            else if ((c & 0xF0) == 0xE0) bytes = 3;
            else if ((c & 0xF8) == 0xF0) bytes = 4;

            for (int b = 0; b < bytes && i + (size_t)b < word.size(); b++)
                result += word[i + b];
            i += bytes;
        }
    }
    while (!result.empty() && result.back() == '-')
        result.pop_back();
    return result;
}

//  1 DALIS – Žodžių dažnumas
void wordFrequency(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream input(inputFile);
    if (!input.is_open()) {
        std::cerr << "Klaida: nepavyko atidaryti " << inputFile << "\n";
        return;
    }

    // map<žodis, kiek kartų pasikartojo>
    std::map<std::string, int> wordCount;

    std::string word;
    while (input >> word) {
        std::string clean = cleanWord(word);
        if (!clean.empty())
            wordCount[clean]++;   // jei žodžio nėra – sukuria su 0, tada +1
    }
    input.close();

    std::ofstream output(outputFile);
    output << "=== ZODZIU DAZNUMAS (pasikartoja > 1 karta) ===\n\n";

    int uniqueCount = 0;
    for (const auto& [zodis, kiekis] : wordCount) {
        if (kiekis > 1) {
            output << zodis << " : " << kiekis << "\n";
            uniqueCount++;
        }
    }
    output << "\n--- Is viso skirtingu zodziu pasikartojusiu >1k: "
        << uniqueCount << " ---\n";

    std::cout << "[1] Zodziu daznumas issaugotas -> " << outputFile << "\n";
}

//  2 DALIS – Cross-reference lentelė
void crossReference(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream input(inputFile);
    if (!input.is_open()) {
        std::cerr << "Klaida: nepavyko atidaryti " << inputFile << "\n";
        return;
    }

    // map<žodis, vector<eilučių numerių>>
    std::map<std::string, std::vector<int>> crossRef;

    std::string line;
    int lineNum = 0;

    while (std::getline(input, line)) {
        lineNum++;
        std::istringstream iss(line);
        std::string word;
        while (iss >> word) {
            std::string clean = cleanWord(word);
            if (!clean.empty())
                crossRef[clean].push_back(lineNum);
        }
    }
    input.close();

    std::ofstream output(outputFile);
    output << "=== CROSS-REFERENCE LENTELE ===\n";
    output << "(Zodžiai pasikartojantys > 1 karta ir ju eilučių numeriai)\n\n";

    for (const auto& [zodis, eilutes] : crossRef) {
        if (eilutes.size() > 1) {
            output << zodis << "\n";
            output << "   Eilutes: ";
            std::vector<int> uniq = eilutes;
            uniq.erase(std::unique(uniq.begin(), uniq.end()), uniq.end());
            for (int el : uniq)
                output << el << " ";
            output << "  (pasikartojimų: " << eilutes.size() << ")\n\n";
        }
    }

    std::cout << "[2] Cross-reference lentele issaugota -> " << outputFile << "\n";
}

//  3 DALIS – URL adresų radimas

void findURLs(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream input(inputFile);
    if (!input.is_open()) {
        std::cerr << "Klaida: nepavyko atidaryti " << inputFile << "\n";
        return;
    }

    // set – automatiškai šalina dublikatus ir rūšiuoja
    std::set<std::string> urls;

    std::regex urlPattern(
        R"((https?://[^\s<>"']+|www\.[a-zA-Z0-9\-]+\.[^\s<>"']+|[a-zA-Z0-9\-]+\.(lt|com|org|net|edu|gov|io|eu)/[^\s<>"']*))",
        std::regex::icase
    );

    std::string line;
    while (std::getline(input, line)) {
        auto begin = std::sregex_iterator(line.begin(), line.end(), urlPattern);
        auto end = std::sregex_iterator();

        for (auto it = begin; it != end; ++it) {
            std::string url = (*it).str();
            // Pašaliname galutinius skyrybos ženklus sakinio gale
            while (!url.empty() && (url.back() == '.' || url.back() == ',' ||
                url.back() == ')' || url.back() == ';'))
                url.pop_back();
            if (!url.empty())
                urls.insert(url);
        }
    }
    input.close();

    std::ofstream output(outputFile);
    output << "=== RASTI URL ADRESAI ===\n\n";

    if (urls.empty()) {
        output << "(URL adresu nerasta)\n";
    }
    else {
        int i = 1;
        for (const auto& url : urls)
            output << i++ << ". " << url << "\n";
    }
    output << "\n--- Is viso URL: " << urls.size() << " ---\n";

    std::cout << "[3] URL adresai issaugoti -> " << outputFile << "\n";
}

//  MAIN
int main() {
    const std::string INPUT = "tekstas.txt";

    std::cout << "\n";
    std::cout << " Asociatyviu konteineriu uzduotis\n";
    std::cout << "\n\n";

    wordFrequency(INPUT, "1_zodziu_daznumas.txt");
    crossReference(INPUT, "2_crossreference.txt");
    findURLs(INPUT, "3_urls.txt");

    std::cout << "\nVisi rezultatai issaugoti!\n";
    return 0;
}