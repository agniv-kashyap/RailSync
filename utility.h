#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <iostream>
#include <limits>

inline void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

inline int getIntInput(const std::string &prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;

        if (std::cin.fail()) {
            std::cin.clear(); // clear failbit
            clearInputBuffer(); // discard invalid input
            std::cout << "Invalid input. Please enter a valid number.\n";
        } else {
            clearInputBuffer(); // discard trailing newline
            return value;
        }
    }
}

#endif // UTILITY_H
