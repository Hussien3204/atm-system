#include "utils/string_utils.h"

namespace StringUtils {

std::string trim(const std::string& str) {
    const std::string whitespace = " \t\r\n";
    const std::size_t first = str.find_first_not_of(whitespace);
    if (first == std::string::npos) {
        return "";
    }
    const std::size_t last = str.find_last_not_of(whitespace);
    return str.substr(first, last - first + 1);
}


// Split a string into tokens based on a specified delimiter, supporting multi-character delimiters.
std::vector<std::string> splitString(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> tokens;
    if (str.empty()) {
        return tokens;
    }
    if (delimiter.empty()) {
        tokens.push_back(str);
        return tokens;
    }

    tokens.reserve(8);
    std::size_t start = 0;
    std::size_t end = str.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }
    tokens.push_back(str.substr(start));
    return tokens;
}




// Join a vector of strings into a single string with a specified delimiter.
std::string joinString(const std::vector<std::string>& tokens, const std::string& delimiter) {
    if (tokens.empty()) {
        return "";
    }

    std::size_t totalLength = 0;
    for (const auto& token : tokens) {
        totalLength += token.length();
    }
    // Safe from unsigned underflow because tokens.empty() is guarded above.
    totalLength += (tokens.size() - 1) * delimiter.length();

    std::string result;
    result.reserve(totalLength);

    for (std::size_t i = 0; i < tokens.size(); ++i) {
        result += tokens.at(i);
        if (i < tokens.size() - 1) {
            result += delimiter;
        }
    }
    return result;
}

} // namespace StringUtils
