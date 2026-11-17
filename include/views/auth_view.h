#ifndef ATM_VIEWS_AUTH_VIEW_H
#define ATM_VIEWS_AUTH_VIEW_H

#include <string>

namespace AuthView {

// Dumb UI layer for ATM authentication screens (Zero business or storage logic).
void showLoginHeader();

void promptCredentials(std::string& outAccountNumber, std::string& outPinCode);

void showInvalidCredentialsWarning(int remainingAttempts);

void showAccountLockedError();

} // namespace AuthView

#endif // ATM_VIEWS_AUTH_VIEW_H
