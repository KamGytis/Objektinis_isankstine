#include "cleanword.h"
#include <cctype>

std::string cleanWord(const std::string& word) {
    std::string result;
    size_t i = 0;
    while (i < word.size()) {
        unsigned char c = (unsigned char)word[i];
        if (c < 0x80) {
            if (std::isalpha(c)) result += (char)std::tolower(c);
            else if (c == '-' && !result.empty() && i + 1 < word.size() && std::isalpha((unsigned char)word[i + 1]))
            result += '-';
            i++;
        }
        else {
            int bytes = 1;
            if ((c & 0xE0) == 0xC0) bytes = 2; // 2 baitu raides lietuviskos kirlica ir t.t
			else if ((c & 0xF0) == 0xE0) bytes = 3; // specialus zenklai, kaip ", ', !, ?, ir tt, gali buti 3 baitai UTF-8 formatu
			else if ((c & 0xF8) == 0xF0) bytes = 4; // 4 baitai UTF-8 formatu, emoji ir tt
            if (bytes == 2) {
                for (int b = 0; b < bytes && i + (size_t)b < word.size(); b++)
                    result += word[i + b];
            }

            i += bytes;
        }
    }
    while (!result.empty() && result.back() == '-') result.pop_back();
    return result;
}
