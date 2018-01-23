#include "Utilities.h"

std::string Utils::toLowerCase(std::string& a_word) {
    std::string lowerCaseWord;

    for (char c : a_word) {
        lowerCaseWord.push_back(tolower(c));
    }

    return lowerCaseWord;
}
