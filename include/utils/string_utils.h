#ifndef ATM_UTILS_STRING_UTILS_H
#define ATM_UTILS_STRING_UTILS_H

#include <string>
#include <vector>

namespace StringUtils {

// String manipulation utilities Supporting multi-character delimiters for safe flat-file parsing.
std::string trim(const std::string& str);

std::vector<std::string> splitString(const std::string& str, const std::string& delimiter);

std::string joinString(const std::vector<std::string>& tokens, const std::string& delimiter);

} // namespace StringUtils

#endif // ATM_UTILS_STRING_UTILS_H
