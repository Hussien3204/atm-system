#include "views/atm_view.h"
#include "utils/input_utils.h"

#include <iomanip>
#include <iostream>

namespace AtmView {

// Algorithm: Clear terminal via ANSI escape codes and print formatted section banner.
// Complexity: Time O(1), Space O(1).
static void printScreenBanner(const std::string& title) {
    std::cout << "\033[2J\033[1;1H";
    std::cout << "========================================\n";
    std::cout << "   " << title << "\n";
    std::cout << "========================================\n";
}

// Algorithm: Render main ATM menu options and cast validated integer [1..5] to MenuOption enum.
// Complexity: Time O(1) per valid input, Space O(1).
MenuOption showMainMenuAndGetChoice(const std::string& clientName) {
    printScreenBanner("ATM MAIN MENU - Welcome, " + clientName);
    std::cout << "  [1] Quick Withdraw\n";
    std::cout << "  [2] Normal Withdraw\n";
    std::cout << "  [3] Deposit\n";
    std::cout << "  [4] Check Balance\n";
    std::cout << "  [5] Logout\n";
    std::cout << "========================================\n";

    const int choice = InputUtils::readIntInRange("Choose an option [1-5]: ", 1, 5);
    return static_cast<MenuOption>(choice);
}

// Algorithm: Display preset quick-withdraw denominations and return user selection [1..9].
// Complexity: Time O(1) per valid input, Space O(1).
int showQuickWithdrawMenuAndGetChoice(double currentBalance) {
    printScreenBanner("QUICK WITHDRAW");
    std::cout << "  [1] 20        [2] 50\n";
    std::cout << "  [3] 100       [4] 200\n";
    std::cout << "  [5] 400       [6] 600\n";
    std::cout << "  [7] 800       [8] 1000\n";
    std::cout << "  [9] Cancel / Back to Main Menu\n";
    std::cout << "----------------------------------------\n";
    std::cout << "Current Balance: $" << std::fixed << std::setprecision(2) << currentBalance << "\n";
    std::cout << "========================================\n";

    return InputUtils::readIntInRange("Choose an option [1-9]: ", 1, 9);
}

// Algorithm: Display normal withdrawal prompt and read non-negative integer amount.
// Complexity: Time O(1) per valid input, Space O(1).
int promptNormalWithdrawAmount(double currentBalance) {
    printScreenBanner("NORMAL WITHDRAW");
    std::cout << "Current Balance: $" << std::fixed << std::setprecision(2) << currentBalance << "\n";
    std::cout << "(Enter 0 to cancel and return to Main Menu)\n";
    std::cout << "----------------------------------------\n";

    return InputUtils::readIntInRange("Enter withdrawal amount (multiples of 5): ", 0, 100000);
}

// Algorithm: Display deposit screen and read validated positive double amount.
// Complexity: Time O(1) per valid input, Space O(1).
double promptDepositAmount(double currentBalance) {
    printScreenBanner("CASH DEPOSIT");
    std::cout << "Current Balance: $" << std::fixed << std::setprecision(2) << currentBalance << "\n";
    std::cout << "----------------------------------------\n";

    return InputUtils::readPositiveDouble("Enter positive deposit amount: $");
}

// Algorithm: Render formatted client account balance summary and wait for Enter key.
// Complexity: Time O(1), Space O(1).
void showBalanceScreen(const std::string& accountNumber, double currentBalance) {
    printScreenBanner("ACCOUNT BALANCE INQUIRY");
    std::cout << "Account Number : " << accountNumber << "\n";
    std::cout << "Current Balance: $" << std::fixed << std::setprecision(2) << currentBalance << "\n";
    std::cout << "========================================\n";
    InputUtils::pauseScreen();
}

// Algorithm: Prompt explicit user confirmation (y/n) before mutating account balance.
// Complexity: Time O(1) per valid input, Space O(1).
bool confirmTransaction(double amount, const std::string& actionName) {
    std::cout << "\nConfirm " << actionName << " of $"
              << std::fixed << std::setprecision(2) << amount << "? (y/n): ";
    return InputUtils::readYesNo("");
}

// Algorithm: Display transaction outcome status along with updated balance and pause screen.
// Complexity: Time O(1), Space O(1).
void showTransactionMessage(bool isSuccess, const std::string& message, double currentBalance) {
    std::cout << "\n" << (isSuccess ? "[SUCCESS] " : "[FAILED] ") << message << "\n";
    std::cout << "Available Balance: $" << std::fixed << std::setprecision(2) << currentBalance << "\n";
    InputUtils::pauseScreen();
}

} // namespace AtmView
