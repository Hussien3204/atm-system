#include "controllers/auth_controller.h"
#include "views/auth_view.h"

#include <cstddef>

namespace AuthController {

static const int MAX_PIN_ATTEMPTS = 3;

// Algorithm: Bitwise XOR accumulation across all bytes to prevent timing side-channel leaks.
// Complexity: Time O(M) where M is input length, Auxiliary Space O(1).
static bool constantTimeEquals(const std::string& a, const std::string& b) {
    unsigned char diff = static_cast<unsigned char>(a.size() != b.size());
    const std::size_t minLen = (a.size() < b.size()) ? a.size() : b.size();

    for (std::size_t i = 0; i < minLen; ++i) {
        const unsigned char byteA = static_cast<unsigned char>(a[i]);
        const unsigned char byteB = static_cast<unsigned char>(b[i]);
        diff = static_cast<unsigned char>(diff | (byteA ^ byteB));
    }

    return diff == 0U;
}

// Algorithm: Iterative credential verification with anti-enumeration strike counter and atomic 3-strike persistent lockout.
// Complexity: Time O(N * L) per attempt for disk lookup/commit, Auxiliary Space O(N * L) on state mutation.
bool authenticateSession(Models::Client& outClient, const std::string& dataPath) {
    int unknownAccountStrikes = 0;

    while (true) {
        AuthView::showLoginHeader();

        std::string accountNumber;
        std::string pinCode;
        AuthView::promptCredentials(accountNumber, pinCode);

        Models::Client candidate;
        if (!ClientRepo::findClientByAccount(accountNumber, candidate, dataPath)) {
            ++unknownAccountStrikes;
            if (unknownAccountStrikes >= MAX_PIN_ATTEMPTS) {
                AuthView::showAccountLockedError();
                return false;
            }
            AuthView::showInvalidCredentialsWarning(MAX_PIN_ATTEMPTS - unknownAccountStrikes);
            continue;
        }

        if (candidate.isLocked || candidate.failedPinAttempts >= MAX_PIN_ATTEMPTS) {
            AuthView::showAccountLockedError();
            return false;
        }

        if (!constantTimeEquals(candidate.pinCode, pinCode)) {
            Models::Client stagedClient = candidate;
            ++stagedClient.failedPinAttempts;
            if (stagedClient.failedPinAttempts >= MAX_PIN_ATTEMPTS) {
                stagedClient.isLocked = true;
            }

            if (!ClientRepo::updateClient(stagedClient, dataPath)) {
                return false;
            }

            if (stagedClient.isLocked) {
                AuthView::showAccountLockedError();
                return false;
            }

            AuthView::showInvalidCredentialsWarning(
                MAX_PIN_ATTEMPTS - stagedClient.failedPinAttempts);
            continue;
        }

        if (candidate.failedPinAttempts > 0) {
            Models::Client stagedClient = candidate;
            stagedClient.failedPinAttempts = 0;
            if (!ClientRepo::updateClient(stagedClient, dataPath)) {
                return false;
            }
            candidate = stagedClient;
        }

        outClient = candidate;
        return true;
    }
}

} // namespace AuthController
