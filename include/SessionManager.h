#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <string>
#include <vector>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPCookie.h>
#include "Database.h"

using namespace std;
using namespace cgicc;

class SessionManager {
private:
    Database* db;
    // SR2: Define timeout duration (e.g., 30 minutes = 1800 seconds)
    const int SESSION_TIMEOUT = 1800; 

    // Helper to generate a random session ID
    string generateSessionId();

public:
    SessionManager(Database* database);
    ~SessionManager();

    // SR3: Create a new session and return the cookie to send to the browser
    HTTPCookie createSession(int userId);

    // SR2 & SR3: Check if a request has a valid session. 
    // Returns userId if valid, 0 if invalid/expired.
    int checkSession(const Cgicc& cgi);

    // SR2: Logout functionality
    void destroySession(const Cgicc& cgi);
};

#endif