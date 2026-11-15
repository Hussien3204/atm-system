#ifndef ATM_UTILS_INPUT_UTILS_H
#define ATM_UTILS_INPUT_UTILS_H

#include <string>

namespace InputUtils {

// Safe console input utilities preventing stream corruption on invalid user input.
std::string readNonEmptyString(const std::string& prompt);

int readIntInRange(const std::string& prompt, int min, int max);

double readPositiveDouble(const std::string& prompt);

bool readYesNo(const std::string& prompt);

void pauseScreen(const std::string& message = "Press Enter to continue...");

} // namespace InputUtils

#endif // ATM_UTILS_INPUT_UTILS_H
