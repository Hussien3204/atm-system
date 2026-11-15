#ifndef ATM_VIEWS_ATM_VIEW_H
#define ATM_VIEWS_ATM_VIEW_H

#include <string>

namespace AtmView {

// Strongly-typed menu options eliminating magic numbers in controller routing.
enum class MenuOption {
    QuickWithdraw = 1,
    NormalWithdraw = 2,
    Deposit = 3,
    CheckBalance = 4,
    Logout = 5
};

// Dumb UI screens for ATM operations (Zero business rules or file access).
MenuOption showMainMenuAndGetChoice(const std::string& clientName);

int showQuickWithdrawMenuAndGetChoice(double currentBalance);

int promptNormalWithdrawAmount(double currentBalance);

double promptDepositAmount(double currentBalance);

void showBalanceScreen(const std::string& accountNumber, double currentBalance);

bool confirmTransaction(double amount, const std::string& actionName);

void showTransactionMessage(bool isSuccess, const std::string& message, double currentBalance);

} // namespace AtmView

#endif // ATM_VIEWS_ATM_VIEW_H
