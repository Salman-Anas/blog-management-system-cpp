#include <iostream>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPRedirectHeader.h>
#include "include/Database.h"
#include "include/SessionManager.h"

using namespace std;
using namespace cgicc;

int main() {
    Cgicc cgi;
    Database db;
    db.connect();
    SessionManager session(&db);

    // SR2: Destroy session in DB and redirect to login
    session.destroySession(cgi);

    cout << HTTPRedirectHeader("login.cgi") << endl;
    return 0;
}