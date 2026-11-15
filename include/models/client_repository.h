#ifndef ATM_MODELS_CLIENT_REPOSITORY_H
#define ATM_MODELS_CLIENT_REPOSITORY_H

#include "models/client.h"

#include <string>
#include <vector>

namespace ClientRepo {

// Default storage path used in production; tests override this with isolated temp files.
const std::string DEFAULT_DATA_PATH = "data/clients.txt";

bool ensureSeedData(const std::string& filePath = DEFAULT_DATA_PATH);
// Loads all clients from the specified file path. Returns an empty vector if the file does not exist or is empty.

std::vector<Models::Client> loadAllClients(const std::string& filePath = DEFAULT_DATA_PATH);

// Saves all clients to the specified file path. Returns true if successful, false otherwise.
bool saveAllClients(const std::vector<Models::Client>& clients,
                    const std::string& filePath = DEFAULT_DATA_PATH);


    // Finds a client by account number. Returns true if found, false otherwise. If found, the client data is copied to outClient.
bool findClientByAccount(const std::string& accountNumber,
                         Models::Client& outClient,
                         const std::string& filePath = DEFAULT_DATA_PATH);

bool updateClient(const Models::Client& updatedClient,
                  const std::string& filePath = DEFAULT_DATA_PATH);

} // namespace ClientRepo

#endif // ATM_MODELS_CLIENT_REPOSITORY_H
