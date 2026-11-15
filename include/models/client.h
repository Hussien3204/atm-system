#ifndef ATM_MODELS_CLIENT_H
#define ATM_MODELS_CLIENT_H

#include <string>

namespace Models {

// Fields are ordered by descending size to minimize struct padding on 64-bit targets.
struct Client {
    std::string accountNumber;
    std::string pinCode;
    std::string fullName;
    std::string phone;
    double balance = 0.0;
    int failedPinAttempts = 0;
    bool isLocked = false;
};

} // namespace Models

#endif // ATM_MODELS_CLIENT_H
