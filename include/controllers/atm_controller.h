#ifndef ATM_CONTROLLERS_ATM_CONTROLLER_H
#define ATM_CONTROLLERS_ATM_CONTROLLER_H

#include "models/client.h"
#include "models/client_repository.h"

#include <string>

namespace AtmController {

// Entry point for an authenticated ATM session event loop (Zero mutual recursion).
void runAtmSession(Models::Client& activeClient,
                   const std::string& dataPath = ClientRepo::DEFAULT_DATA_PATH);

} // namespace AtmController

#endif // ATM_CONTROLLERS_ATM_CONTROLLER_H
