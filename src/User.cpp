#include "../include/User.h"
#include <iostream>
#include <openssl/evp.h>

User::User(Database* database) {
    this->db = database;
    this->id = 0;
}

User::~User() {}

// SR5: SHA256 Hashing Implementation
string User::hashPassword(string password) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int lengthOfHash = 0;

    EVP_MD_CTX* context = EVP_MD_CTX_new();
    
    if(context != NULL) {
        if(EVP_DigestInit_ex(context, EVP_sha256(), NULL)) {
            if(EVP_DigestUpdate(context, password.c_str(), password.length())) {
                EVP_DigestFinal_ex(context, hash, &lengthOfHash);
            }
        }
        EVP_MD_CTX_free(context);
    }

    stringstream ss;
    for(unsigned int i = 0; i < lengthOfHash; i++) {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}

int User::authenticate(string inputUsername, string inputPassword) {
    // 1. Sanitize input (SR6)
    string cleanUser = db->sanitize(inputUsername);
    
    // 2. Hash the input password (SR5)
    string hashedPassword = hashPassword(inputPassword);

    // 3. Query DB
    string query = "SELECT id, email, role FROM users WHERE username = '" + cleanUser + "' AND password_hash = '" + hashedPassword + "'";
    
    MYSQL_RES* res = db->select(query);
    if (!res) return 0;

    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) return 0; // Invalid credentials

    // 4. Success - Load data
    this->id = stoi(row[0]);
    this->email = row[1];
    this->role = row[2];
    this->username = cleanUser;

    return this->id;
}

bool User::load(int userId) {
    string query = "SELECT username, email, role FROM users WHERE id = " + to_string(userId);
    MYSQL_RES* res = db->select(query);
    if (!res) return false;

    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) return false;

    this->id = userId;
    this->username = row[0];
    this->email = row[1];
    this->role = row[2];
    
    return true;
}