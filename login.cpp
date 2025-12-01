#include <iostream>
#include <string>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTTPRedirectHeader.h> 
#include <cgicc/HTMLClasses.h>
#include "include/Database.h"
#include "include/SessionManager.h"
#include "include/User.h"

using namespace std;
using namespace cgicc;

int main() {
    Cgicc cgi;
    Database db;
    db.connect();
    SessionManager session(&db);
    User user(&db);

    string error_msg = "";

    // 1. Check if form was submitted (POST request)
    // SR1: User attempts to log in
    const_form_iterator userIter = cgi.getElement("username");
    const_form_iterator passIter = cgi.getElement("password");

    if (userIter != cgi.getElements().end() && passIter != cgi.getElements().end()) {
        string u_name = userIter->getValue();
        string u_pass = passIter->getValue();

        int userId = user.authenticate(u_name, u_pass);

        if (userId > 0) {
            // SUCCESS: Create Session and Redirect (SR3, SR4)
            HTTPCookie cookie = session.createSession(userId);
            
            // FIX: Use the specific Redirect Header class
            HTTPRedirectHeader redirect("dashboard.cgi");
            redirect.setCookie(cookie);
            
            cout << redirect << endl;
            return 0;
        } else {
            error_msg = "Invalid username or password.";
        }
    }

    // 2. Render HTML Form (GET request or Failed Login)
    cout << HTTPHTMLHeader() << endl;
    cout << html() << head(title("Login - Blog System")) << endl;
    
    // Simple CSS for maintainability
    cout << style("body { font-family: sans-serif; max-width: 400px; margin: 50px auto; padding: 20px; border: 1px solid #ccc; }") << endl;
    
    cout << body() << h1("Login") << endl;

    if (!error_msg.empty()) {
        cout << p(error_msg) << style("color:red") << endl;
    }

    cout << form().set("method", "post").set("action", "login.cgi") << endl;
    
    cout << p("Username:") << input().set("type", "text").set("name", "username").set("required", "true") << endl;
    cout << p("Password:") << input().set("type", "password").set("name", "password").set("required", "true") << endl;
    
    cout << input().set("type", "submit").set("value", "Login") << endl;
    
    cout << form() << body() << html() << endl;

    return 0;
}