#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <openssl/sha.h> // For SR5 (Encryption)
#include <iomanip>
#include <sstream>
#include "Database.h"

using namespace std;

class User {
private:
    Database* db;
    
    // SR5: Helper to hash passwords using SHA256
    string hashPassword(string password);

public:
    int id;
    string username;
    string email;
    string role;

    User(Database* database);
    ~User();

    // SR1: Verify username and password. Returns user ID if valid, 0 if invalid.
    int authenticate(string username, string password);
    
    // Helper to get user details by ID
    bool load(int userId);

    bool createUser(string username, string password, string email, string role);
};

#endif