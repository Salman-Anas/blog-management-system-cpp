#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>
#include "Database.h"

using namespace std;

class User {
private:
    Database* db;
    string hashPassword(string password);

public:
    int id;
    string username;
    string email;
    string role;

    User(Database* database);
    ~User();
    int authenticate(string username, string password);
    bool load(int userId);
    bool createUser(string username, string password, string email, string role);
    void set2FACode(int userId, string code);
    bool verify2FACode(int userId, string inputCode);
};

#endif