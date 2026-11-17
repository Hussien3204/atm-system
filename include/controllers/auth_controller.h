#ifndef ATM_CONTROLLERS_AUTH_CONTROLLER_H
#define ATM_CONTROLLERS_AUTH_CONTROLLER_H

#include "models/client.h"
#include "models/client_repository.h"

#include <string>

namespace AuthController {

// Authenticates a client session and enforces persistent 3-strike card lockout.
bool authenticateSession(Models::Client& outClient,
                         const std::string& dataPath = ClientRepo::DEFAULT_DATA_PATH);

} // namespace AuthController

#endif // ATM_CONTROLLERS_AUTH_CONTROLLER_H
