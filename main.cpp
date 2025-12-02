#include <iostream>
#include <string>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>
#include "include/Database.h"
#include "include/SessionManager.h" 

using namespace std;
using namespace cgicc;

int main() {
    Cgicc cgi;
    Database db;
    db.connect();
    SessionManager session(&db);
    int userId = session.checkSession(cgi);

    if (userId == 0) {
        HTTPCookie newCookie = session.createSession(1);
        cout << HTTPHTMLHeader().setCookie(newCookie) << endl;    
        cout << html() << head(title("Session Test")) << body() << endl;
        cout << h1("Session Created") << endl;
        cout << p("No session found. I have created a new session for User ID 1.") << endl;
        cout << p("Refresh the page to see if the session persists.") << endl;
    }else{
        cout << HTTPHTMLHeader() << endl;
        cout << html() << head(title("Session Test")) << body() << endl;
        cout << h1("Welcome Back!") << endl;
        cout << p("Session Valid. Your User ID is: " + to_string(userId)) << style("color:green") << endl;
        cout << p("Because you refreshed, the timeout timer has been reset (SR2).") << endl;
    }

    cout << body() << html() << endl;
    return 0;
}