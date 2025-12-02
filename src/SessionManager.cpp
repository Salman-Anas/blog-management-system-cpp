#include "../include/SessionManager.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <cgicc/CgiEnvironment.h> 

using namespace cgicc;

SessionManager::SessionManager(Database* database) {
    this->db = database;
    srand(time(0)); 
}

SessionManager::~SessionManager() {
}

string SessionManager::generateSessionId() {
    const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    string token;
    for (int i = 0; i < 32; i++) {
        token += charset[rand() % (sizeof(charset) - 1)];
    }
    return token;
}

HTTPCookie SessionManager::createSession(int userId) {
    string token = generateSessionId();
    string query = "INSERT INTO sessions (session_token, user_id) VALUES ('" + token + "', " + to_string(userId) + ")";
    db->execute(query);
    HTTPCookie cookie("BLOG_SESSION", token);
    cookie.setMaxAge(SESSION_TIMEOUT);
    cookie.setPath("/");
    
    return cookie;
}

int SessionManager::checkSession(const Cgicc& cgi) {
    const CgiEnvironment& env = cgi.getEnvironment();
    const_cookie_iterator iter = env.getCookieList().begin();
    string token = "";
    for (; iter != env.getCookieList().end(); ++iter) {
        if (iter->getName() == "BLOG_SESSION") {
            token = iter->getValue();
            break;
        }
    }
    if (token.empty()) return 0; 
    string cleanToken = db->sanitize(token);
    string query = "SELECT user_id, TIMESTAMPDIFF(SECOND, last_activity, NOW()) as inactivity FROM sessions WHERE session_token = '" + cleanToken + "'";

    MYSQL_RES* res = db->select(query);
    if (!res) return 0;
    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) return 0;
    int userId = stoi(row[0]);
    int inactivity = stoi(row[1]);
    if (inactivity > SESSION_TIMEOUT) {
        destroySession(cgi);
        return 0;
    }
    db->execute("UPDATE sessions SET last_activity = NOW() WHERE session_token = '" + cleanToken + "'");

    return userId;
}

void SessionManager::destroySession(const Cgicc& cgi){
    const CgiEnvironment& env = cgi.getEnvironment();
    const_cookie_iterator iter = env.getCookieList().begin();

    for (; iter != env.getCookieList().end(); ++iter) {
        if (iter->getName() == "BLOG_SESSION") {
            string token = db->sanitize(iter->getValue());
            db->execute("DELETE FROM sessions WHERE session_token = '" + token + "'");
            return;
        }
    }
}