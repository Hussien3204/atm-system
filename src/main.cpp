#include "controllers/atm_controller.h"
#include "controllers/auth_controller.h"
#include "models/client.h"
#include "models/client_repository.h"

#include <cstdlib>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

// Algorithm: Enable Windows Console Virtual Terminal Processing bitmask if running on Win32 host.
// Complexity: Time O(1), Space O(1).
static void enableAnsiTerminal() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            SetConsoleMode(hOut, dwMode | 0x0004);
        }
    }
#endif
}

// Algorithm: Bootstrap persistent seed storage, then run iterative authenticate-and-serve kiosk loop without recursion.
// Complexity: Time O(S * K * N * L) across S sessions and K operations, Auxiliary Stack Space O(1).
int main() {
    enableAnsiTerminal();

    const std::string dataPath = ClientRepo::DEFAULT_DATA_PATH;

    if (!ClientRepo::ensureSeedData(dataPath)) {
        std::cerr << "Fatal Error: Unable to initialize storage at " << dataPath << '\n';
        return EXIT_FAILURE;
    }

    while (std::cin.good()) {
        Models::Client activeClient;

        if (!AuthController::authenticateSession(activeClient, dataPath)) {
            return EXIT_FAILURE;
        }

        AtmController::runAtmSession(activeClient, dataPath);
    }

    return EXIT_SUCCESS;
}
