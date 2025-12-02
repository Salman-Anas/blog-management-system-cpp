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
    const int SESSION_TIMEOUT = 1800; 
    string generateSessionId();

public:
    SessionManager(Database* database);
    ~SessionManager();
    HTTPCookie createSession(int userId);
    int checkSession(const Cgicc& cgi);
    void destroySession(const Cgicc& cgi);
};

#endif