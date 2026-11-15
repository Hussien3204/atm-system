#include "utils/input_utils.h"
#include "utils/string_utils.h"

#include <cmath>
#include <iostream>

namespace InputUtils {

std::string readNonEmptyString(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string input;
        if (!std::getline(std::cin, input)) {
            std::cin.clear();
            continue;
        }
        input = StringUtils::trim(input);
        if (!input.empty()) {
            return input;
        }
        std::cout << "Invalid input. Value cannot be empty.\n";
    }
}

int readIntInRange(const std::string& prompt, int min, int max) {
    while (true) {
        const std::string raw = readNonEmptyString(prompt);
        try {
            std::size_t parsedChars = 0;
            const int value = std::stoi(raw, &parsedChars);
            if (parsedChars == raw.size() && value >= min && value <= max) {
                return value;
            }
            std::cout << "Invalid choice. Please enter a number between "
                      << min << " and " << max << ".\n";
        } catch (...) {
            std::cout << "Invalid input. Please enter a valid integer.\n";
        }
    }
}

double readPositiveDouble(const std::string& prompt) {
    while (true) {
        const std::string raw = readNonEmptyString(prompt);
        try {
            std::size_t parsedChars = 0;
            const double value = std::stod(raw, &parsedChars);
            // Guard against NaN/Inf literals accepted by std::stod.
            if (parsedChars == raw.size() && std::isfinite(value) && value > 0.0) {
                return value;
            }
            std::cout << "Invalid amount. Value must be greater than zero.\n";
        } catch (...) {
            std::cout << "Invalid input. Please enter a valid numeric amount.\n";
        }
    }
}

bool readYesNo(const std::string& prompt) {
    while (true) {
        const std::string input = readNonEmptyString(prompt);
        if (input == "y" || input == "Y") {
            return true;
        }
        if (input == "n" || input == "N") {
            return false;
        }
        std::cout << "Invalid input. Please enter 'y' for yes or 'n' for no.\n";
    }
}

void pauseScreen(const std::string& message) {
    std::cout << message;
    std::string dummy;
    if (!std::getline(std::cin, dummy)) {
        std::cin.clear();
    }
}

} // namespace InputUtils
