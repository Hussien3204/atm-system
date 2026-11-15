#include "models/client_repository.h"
#include "utils/string_utils.h"

#include <cstdio>
#include <fstream>
#include <iomanip>
#include <sstream>

namespace ClientRepo {

static const std::string DELIMITER = "#//#";

// Algorithm: Format balance to 2 decimal places, pack 7 fields, and join with delimiter.
// Complexity: Time O(L), Space O(L) where L is the serialized record length.
static std::string serializeClient(const Models::Client& client) {
    std::ostringstream balanceStream;
    balanceStream << std::fixed << std::setprecision(2) << client.balance;

    const std::vector<std::string> fields = {
        client.accountNumber,
        client.pinCode,
        client.fullName,
        client.phone,
        balanceStream.str(),
        std::to_string(client.failedPinAttempts),
        client.isLocked ? "1" : "0"
    };
    return StringUtils::joinString(fields, DELIMITER);
}

// Algorithm: Trim raw line, split by delimiter, validate field count (7), and parse primitives safely.
// Complexity: Time O(L), Space O(L) where L is the raw line length.
static bool deserializeClient(const std::string& line, Models::Client& outClient) {
    const std::string cleanLine = StringUtils::trim(line);
    if (cleanLine.empty()) {
        return false;
    }

    const std::vector<std::string> fields = StringUtils::splitString(cleanLine, DELIMITER);
    if (fields.size() != 7) {
        return false;
    }

    try {
        outClient.accountNumber = fields.at(0);
        outClient.pinCode = fields.at(1);
        outClient.fullName = fields.at(2);
        outClient.phone = fields.at(3);
        outClient.balance = std::stod(fields.at(4));
        outClient.failedPinAttempts = std::stoi(fields.at(5));
        outClient.isLocked = (fields.at(6) == "1");
        return true;
    } catch (...) {
        return false;
    }
}

// Algorithm: Construct a populated Client POD instance explicitly for strict C++11 compatibility.
// Complexity: Time O(1), Space O(1).
static Models::Client makeSeedClient(const std::string& accountNumber,
                                     const std::string& pinCode,
                                     const std::string& fullName,
                                     const std::string& phone,
                                     double balance) {
    Models::Client client;
    client.accountNumber = accountNumber;
    client.pinCode = pinCode;
    client.fullName = fullName;
    client.phone = phone;
    client.balance = balance;
    client.failedPinAttempts = 0;
    client.isLocked = false;
    return client;
}

// Algorithm: Atomic write-replace — write all records to a .tmp file under RAII, verify stream health, then replace target.
// Complexity: Time O(N * L), Space O(L) auxiliary memory where N is client count.
bool saveAllClients(const std::vector<Models::Client>& clients, const std::string& filePath) {
    const std::string tempPath = filePath + ".tmp";
    {
        std::ofstream outFile(tempPath, std::ios::trunc);
        if (!outFile.is_open()) {
            return false;
        }
        for (std::size_t i = 0; i < clients.size(); ++i) {
            outFile << serializeClient(clients.at(i)) << "\n";
        }
        outFile.flush();
        if (!outFile.good()) {
            return false;
        }
    }

    std::remove(filePath.c_str());
    return std::rename(tempPath.c_str(), filePath.c_str()) == 0;
}

// Algorithm: Pre-reserve capacity, stream file line-by-line, and deserialize valid records.
// Complexity: Time O(N * L), Space O(N * L) to hold all loaded client structs.
std::vector<Models::Client> loadAllClients(const std::string& filePath) {
    std::vector<Models::Client> clients;
    std::ifstream inFile(filePath);
    if (!inFile.is_open()) {
        return clients;
    }

    clients.reserve(64);
    std::string line;
    while (std::getline(inFile, line)) {
        Models::Client client;
        if (deserializeClient(line, client)) {
            clients.push_back(client);
        }
    }
    return clients;
}

// Algorithm: Check if file exists and is non-empty via peek(); otherwise persist 3 default seed accounts.
// Complexity: Time O(1) when file exists, O(K) on first boot where K = 3 seed records; Space O(1).
bool ensureSeedData(const std::string& filePath) {
    {
        std::ifstream checkFile(filePath);
        if (checkFile.is_open() && checkFile.peek() != std::ifstream::traits_type::eof()) {
            return true;
        }
    }

    std::vector<Models::Client> seedClients;
    seedClients.reserve(3);
    seedClients.push_back(makeSeedClient("A101", "1234", "Hussien Al-Ahmad", "0991112233", 5000.00));
    seedClients.push_back(makeSeedClient("A102", "4321", "Sara Mahmoud", "0992223344", 12500.50));
    seedClients.push_back(makeSeedClient("A103", "0000", "Khaled Mansour", "0993334455", 350.00));
    return saveAllClients(seedClients, filePath);
}

// Algorithm: Stream file line-by-line with early exit on match without loading entire dataset into RAM.
// Complexity: Time O(N * L) worst-case / O(L) best-case, Space O(L) constant single-record memory.
bool findClientByAccount(const std::string& accountNumber,
                         Models::Client& outClient,
                         const std::string& filePath) {
    std::ifstream inFile(filePath);
    if (!inFile.is_open()) {
        return false;
    }

    std::string line;
    Models::Client candidate;
    while (std::getline(inFile, line)) {
        if (deserializeClient(line, candidate) && candidate.accountNumber == accountNumber) {
            outClient = candidate;
            return true;
        }
    }
    return false;
}

// Algorithm: Load all records, replace matching account in-place, and commit atomically; abort if missing.
// Complexity: Time O(N * L), Space O(N * L) required for safe flat-file atomic rewrite.
bool updateClient(const Models::Client& updatedClient, const std::string& filePath) {
    std::vector<Models::Client> clients = loadAllClients(filePath);
    for (auto& client : clients) {
        if (client.accountNumber == updatedClient.accountNumber) {
            client = updatedClient;
            return saveAllClients(clients, filePath);
        }
    }
    return false;
}

} // namespace ClientRepo
