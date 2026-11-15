#include "views/auth_view.h"
#include "utils/input_utils.h"

#include <iostream>

namespace AuthView {

// Algorithm: Clear terminal buffer via ANSI escape sequence and render ATM login banner.
// Complexity: Time O(1), Space O(1).
void showLoginHeader() {
    std::cout << "\033[2J\033[1;1H";
    std::cout << "========================================\n";
    std::cout << "            ATM LOGIN SCREEN            \n";
    std::cout << "========================================\n";
}

// Algorithm: Read non-empty trimmed account number and PIN code into output references.
// Complexity: Time O(L) where L is user input length, Space O(L).
void promptCredentials(std::string& outAccountNumber, std::string& outPinCode) {
    outAccountNumber = InputUtils::readNonEmptyString("Enter Account Number: ");
    outPinCode = InputUtils::readNonEmptyString("Enter PIN Code: ");
}

// Algorithm: Display authentication failure alert along with remaining attempts before lockout.
// Complexity: Time O(1), Space O(1).
void showInvalidCredentialsWarning(int remainingAttempts) {
    std::cout << "\n[!] Invalid credentials. Remaining attempts before lockout: "
              << remainingAttempts << "\n";
    InputUtils::pauseScreen();
}

// Algorithm: Display security lockout message and pause screen so the user can read it.
// Complexity: Time O(1), Space O(1).
void showAccountLockedError() {
    std::cout << "\n[LOCKED] Account is locked due to multiple failed login attempts.\n"
              << "Please contact bank support to unlock your card.\n";
    InputUtils::pauseScreen();
}

} // namespace AuthView
