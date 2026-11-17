#include "controllers/atm_controller.h"
#include "models/client_repository.h"
#include "views/atm_view.h"

#include <array>
#include <cstddef>

namespace AtmController {

static const double EPSILON = 1e-6;

// Algorithm: Validate sufficient funds with IEEE-754 epsilon, confirm with user, commit to disk first, then mutate session state.
// Complexity: Time O(N * L) for atomic disk commit, Space O(N * L).
static void executeWithdrawal(Models::Client& activeClient,
                              double amount,
                              const std::string& dataPath) {
    if (amount > (activeClient.balance + EPSILON)) {
        AtmView::showTransactionMessage(
            false, "Insufficient funds for this withdrawal.", activeClient.balance);
        return;
    }

    if (!AtmView::confirmTransaction(amount, "withdrawal")) {
        AtmView::showTransactionMessage(
            false, "Transaction cancelled by user.", activeClient.balance);
        return;
    }

    Models::Client stagedClient = activeClient;
    stagedClient.balance -= amount;
    if (stagedClient.balance < EPSILON) {
        stagedClient.balance = 0.0;
    }

    if (!ClientRepo::updateClient(stagedClient, dataPath)) {
        AtmView::showTransactionMessage(
            false, "Storage error: could not persist transaction.", activeClient.balance);
        return;
    }

    activeClient = stagedClient;
    AtmView::showTransactionMessage(
        true, "Please collect your cash.", activeClient.balance);
}

// Algorithm: Map menu choice [1..8] via O(1) lookup table to preset amount, or abort on option 9.
// Complexity: Time O(N * L) on execution, Space O(N * L).
static void handleQuickWithdraw(Models::Client& activeClient, const std::string& dataPath) {
    static const std::array<double, 8> PRESET_AMOUNTS = {{
        20.0, 50.0, 100.0, 200.0, 400.0, 600.0, 800.0, 1000.0
    }};

    const int choice = AtmView::showQuickWithdrawMenuAndGetChoice(activeClient.balance);
    if (choice == 9) {
        return;
    }

    if (choice < 1 || choice > 8) {
        AtmView::showTransactionMessage(
            false, "Invalid quick withdraw selection.", activeClient.balance);
        return;
    }

    const std::size_t index = static_cast<std::size_t>(choice - 1);
    executeWithdrawal(activeClient, PRESET_AMOUNTS[index], dataPath);
}

// Algorithm: Prompt custom amount, allow 0 to cancel, enforce multiples-of-5 rule in a loop, then delegate to executeWithdrawal.
// Complexity: Time O(N * L) on execution, Space O(N * L).
static void handleNormalWithdraw(Models::Client& activeClient, const std::string& dataPath) {
    while (true) {
        const int amount = AtmView::promptNormalWithdrawAmount(activeClient.balance);
        if (amount == 0) {
            return;
        }

        if (amount % 5 != 0) {
            AtmView::showTransactionMessage(
                false, "Amount must be a multiple of $5.", activeClient.balance);
            continue;
        }

        executeWithdrawal(activeClient, static_cast<double>(amount), dataPath);
        return;
    }
}

// Algorithm: Prompt positive deposit amount, confirm, commit staged balance atomically to disk first, then update RAM session.
// Complexity: Time O(N * L) for atomic disk commit, Space O(N * L).
static void handleDeposit(Models::Client& activeClient, const std::string& dataPath) {
    const double amount = AtmView::promptDepositAmount(activeClient.balance);
    if (amount <= EPSILON) {
        return;
    }

    if (!AtmView::confirmTransaction(amount, "deposit")) {
        AtmView::showTransactionMessage(
            false, "Transaction cancelled by user.", activeClient.balance);
        return;
    }

    Models::Client stagedClient = activeClient;
    stagedClient.balance += amount;

    if (!ClientRepo::updateClient(stagedClient, dataPath)) {
        AtmView::showTransactionMessage(
            false, "Storage error: could not persist transaction.", activeClient.balance);
        return;
    }

    activeClient = stagedClient;
    AtmView::showTransactionMessage(
        true, "Cash deposited successfully.", activeClient.balance);
}

// Algorithm: Clean while-loop event dispatcher routing strongly-typed MenuOption until Logout (O(1) stack frames).
// Complexity: Time O(K * N * L) across K user operations, Auxiliary Stack Space O(1).
void runAtmSession(Models::Client& activeClient, const std::string& dataPath) {
    bool sessionActive = true;
    while (sessionActive) {
        const AtmView::MenuOption choice =
            AtmView::showMainMenuAndGetChoice(activeClient.fullName);

        switch (choice) {
            case AtmView::MenuOption::QuickWithdraw:
                handleQuickWithdraw(activeClient, dataPath);
                break;
            case AtmView::MenuOption::NormalWithdraw:
                handleNormalWithdraw(activeClient, dataPath);
                break;
            case AtmView::MenuOption::Deposit:
                handleDeposit(activeClient, dataPath);
                break;
            case AtmView::MenuOption::CheckBalance:
                AtmView::showBalanceScreen(activeClient.accountNumber, activeClient.balance);
                break;
            case AtmView::MenuOption::Logout:
                sessionActive = false;
                break;
        }
    }
}

} // namespace AtmController
