#include <iostream>
#include "wordfrequency.h"
#include "crossreference.h"
#include "urls.h"

int main() {
    const std::string INPUT = "tekstas.txt";
    std::cout << "Asociatyviu konteineriu uzduotis\n\n";
    wordFrequency(INPUT, "1_zodziu_daznumas.txt");
    crossReference(INPUT, "2_crossreference.txt");
    findURLs(INPUT, "3_urls.txt");
    std::cout << "\nVisi rezultatai issaugoti!\n";
    return 0;
}