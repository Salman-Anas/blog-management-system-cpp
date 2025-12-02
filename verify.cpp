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

    // 1. Get the PRE_AUTH_USER cookie
    int userId = 0;
    const CgiEnvironment& env = cgi.getEnvironment();
    const_cookie_iterator iter = env.getCookieList().begin();
    for (; iter != env.getCookieList().end(); ++iter) {
        if (iter->getName() == "PRE_AUTH_USER") {
            userId = stoi(iter->getValue());
            break;
        }
    }

    if (userId == 0) {
        // No pre-auth cookie? Go back to start.
        cout << HTTPRedirectHeader("login.cgi") << endl;
        return 0;
    }

    string error_msg = "";

    // 2. Handle Form Submission
    const_form_iterator codeIter = cgi.getElement("code");
    if (codeIter != cgi.getElements().end()) {
        string inputCode = codeIter->getValue();

        if (user.verify2FACode(userId, inputCode)) {
            // SUCCESS STEP 2: Code Valid!
            // Now we create the REAL session (SR3)
            HTTPCookie sessionCookie = session.createSession(userId);
            
            cout << HTTPRedirectHeader("dashboard.cgi")
                    .setCookie(sessionCookie) << endl;
            return 0;
        } else {
            error_msg = "Invalid Code. Check email_log.txt";
        }
    }

    // 3. Render Form
    cout << HTTPHTMLHeader() << endl;
    cout << html() << head(title("Two-Factor Authentication")) << endl;
    cout << style("body { font-family: sans-serif; max-width: 400px; margin: 50px auto; padding: 20px; border: 1px solid #ccc; }") << endl;
    cout << body() << h1("Verify Identity") << endl;

    cout << p("We have sent a verification code to your email (simulated in tmp/email_log.txt).") << endl;

    if (!error_msg.empty()) {
        cout << p(error_msg) << style("color:red") << endl;
    }

    cout << form().set("method", "post").set("action", "verify.cgi") << endl;
    cout << b("Enter 6-Digit Code:") << br();
    cout << input().set("type", "text").set("name", "code").set("required", "true") << endl;
    cout << input().set("type", "submit").set("value", "Verify") << endl;
    cout << form() << body() << html() << endl;

    return 0;
}