#include "../include/User.h"
#include <iostream>
#include <fstream> 
#include <iomanip>
#include <sstream>
#include <openssl/evp.h>

using namespace std;

User::User(Database* database) {
    this->db = database;
    this->id = 0;
}

User::~User() {}
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
    string cleanUser = db->sanitize(inputUsername);
    string hashedPassword = hashPassword(inputPassword);
    string query = "SELECT id, email, role FROM users WHERE username = '" + cleanUser + "' AND password_hash = '" + hashedPassword + "'";
    MYSQL_RES* res = db->select(query);
    if (!res) return 0;
    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) return 0;
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

bool User::createUser(string username, string password, string email, string role) {
    string cleanUser = db->sanitize(username);
    string cleanEmail = db->sanitize(email);
    string cleanRole = db->sanitize(role);

    string hashedPassword = hashPassword(password);

    string query = "INSERT INTO users (username, password_hash, email, role) VALUES ('" 
                   + cleanUser + "', '" + hashedPassword + "', '" + cleanEmail + "', '" + cleanRole + "')";
    
    return db->execute(query);
}

void User::set2FACode(int userId, string code) {
    string cleanCode = db->sanitize(code);
    string query = "UPDATE users SET two_factor_code = '" + cleanCode + "' WHERE id = " + to_string(userId);
    db->execute(query);
    load(userId);
    
    ofstream mailSpool("/tmp/email_log.txt", ios::app);
    if (mailSpool.is_open()) {
        mailSpool << "------------------------------------------------\n";
        mailSpool << "To: " << this->email << "\n";
        mailSpool << "Subject: Your Login Code\n";
        mailSpool << "Your verification code is: " << code << "\n";
        mailSpool << "------------------------------------------------\n\n";
        mailSpool.close();
    }
}

bool User::verify2FACode(int userId, string inputCode) {
    string cleanCode = db->sanitize(inputCode);
    string query = "SELECT id FROM users WHERE id = " + to_string(userId) + " AND two_factor_code = '" + cleanCode + "'";
    
    MYSQL_RES* res = db->select(query);
    if (!res) return false;
    
    MYSQL_ROW row = mysql_fetch_row(res);
    return (row != NULL);
}