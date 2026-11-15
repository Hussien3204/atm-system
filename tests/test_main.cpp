#include "models/client.h"
#include "models/client_repository.h"
#include "utils/string_utils.h"

#include <cassert>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

static const std::string TEST_DB_PATH = "tests/temp_test_clients.txt";

// Algorithm: Compare two IEEE-754 doubles within a strict tolerance epsilon.
// Complexity: Time O(1), Space O(1).
static bool areDoublesEqual(double a, double b, double epsilon = 1e-6) {
    return std::fabs(a - b) < epsilon;
}

// Algorithm: Verify trim, splitString, and joinString across normal and edge cases.
// Complexity: Time O(1) fixed test inputs, Space O(1).
static void testStringUtils() {
    assert(StringUtils::trim("   A101 \t\n ") == "A101");
    assert(StringUtils::trim("     ").empty());

    const std::vector<std::string> tokens = StringUtils::splitString("A101#//#1234#//#5000.00", "#//#");
    assert(tokens.size() == 3);
    assert(tokens.at(0) == "A101");
    assert(tokens.at(1) == "1234");
    assert(tokens.at(2) == "5000.00");

    assert(StringUtils::joinString(tokens, "#//#") == "A101#//#1234#//#5000.00");
    assert(StringUtils::joinString({}, "#//#").empty());
    std::cout << "[PASS] StringUtils unit tests\n";
}

// Algorithm: Seed isolated test file, verify record count, and test O(L)-space lookup for existing and missing accounts.
// Complexity: Time O(1) fixed seed dataset, Space O(1).
static void testSeedAndLookup() {
    std::remove(TEST_DB_PATH.c_str());

    assert(ClientRepo::ensureSeedData(TEST_DB_PATH));

    const std::vector<Models::Client> allClients = ClientRepo::loadAllClients(TEST_DB_PATH);
    assert(allClients.size() == 3);

    Models::Client foundClient;
    assert(ClientRepo::findClientByAccount("A101", foundClient, TEST_DB_PATH));
    assert(foundClient.pinCode == "1234");
    assert(foundClient.fullName == "Hussien Al-Ahmad");
    assert(areDoublesEqual(foundClient.balance, 5000.00));
    assert(foundClient.failedPinAttempts == 0);
    assert(!foundClient.isLocked);

    Models::Client missingClient;
    assert(!ClientRepo::findClientByAccount("Z999", missingClient, TEST_DB_PATH));

    std::remove(TEST_DB_PATH.c_str());
    std::cout << "[PASS] ClientRepo seed & O(L)-space streaming lookup tests\n";
}

// Algorithm: Modify client balance and lockout state, commit via atomic updateClient, and reload from disk to verify persistence.
// Complexity: Time O(1) fixed seed dataset, Space O(1).
static void testUpdateAndLockoutPersistence() {
    std::remove(TEST_DB_PATH.c_str());
    assert(ClientRepo::ensureSeedData(TEST_DB_PATH));

    Models::Client client;
    assert(ClientRepo::findClientByAccount("A101", client, TEST_DB_PATH));

    client.balance -= 500.00;
    client.failedPinAttempts = 3;
    client.isLocked = true;
    assert(ClientRepo::updateClient(client, TEST_DB_PATH));

    Models::Client reloadedClient;
    assert(ClientRepo::findClientByAccount("A101", reloadedClient, TEST_DB_PATH));
    assert(areDoublesEqual(reloadedClient.balance, 4500.00));
    assert(reloadedClient.failedPinAttempts == 3);
    assert(reloadedClient.isLocked);

    Models::Client ghostClient;
    ghostClient.accountNumber = "GHOST_999";
    assert(!ClientRepo::updateClient(ghostClient, TEST_DB_PATH));

    std::remove(TEST_DB_PATH.c_str());
    std::cout << "[PASS] ClientRepo atomic update & lockout persistence tests\n";
}

int main() {
    std::cout << "========================================\n";
    std::cout << "   Running ATM Core Isolated Unit Tests \n";
    std::cout << "========================================\n";

    testStringUtils();
    testSeedAndLookup();
    testUpdateAndLockoutPersistence();

    std::cout << "========================================\n";
    std::cout << "   ALL UNIT TESTS PASSED SUCCESSFULLY!  \n";
    std::cout << "========================================\n";
    return 0;
}
