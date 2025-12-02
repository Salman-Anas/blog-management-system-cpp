#include <iostream>
#include <string>
#include <vector>
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
    int userId = session.checkSession(cgi);
    if (userId == 0) {
        cout << HTTPRedirectHeader("login.cgi") << endl;
        return 0;
    }
    if (!user.load(userId) || user.role != "admin") {
        cout << HTTPRedirectHeader("dashboard.cgi") << endl;
        return 0;
    }
    
    const_form_iterator userIter = cgi.getElement("username");
    const_form_iterator passIter = cgi.getElement("password");
    if (userIter != cgi.getElements().end() && passIter != cgi.getElements().end()) {
        string newName = userIter->getValue();
        string newPass = passIter->getValue();
        string newEmail = cgi.getElement("email")->getValue();
        string newRole = cgi.getElement("role")->getValue();

        if (user.createUser(newName, newPass, newEmail, newRole)) {
            cout << HTTPRedirectHeader("admin.cgi") << endl;
            return 0;
        }
    }
    cout << HTTPHTMLHeader() << endl;
    cout << html() << head(title("Admin Panel")) << endl;
    cout << style("body { font-family: sans-serif; max-width: 800px; margin: 40px auto; padding: 20px; }"
                  "table { width: 100%; border-collapse: collapse; margin-top: 20px; }"
                  "th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }"
                  "th { background-color: #333; color: white; }"
                  ".form-box { background: #f4f4f4; padding: 20px; border-radius: 5px; }") << endl;
    cout << body() << endl;
    cout << cgicc::div().set("class", "nav") << endl;
    cout << a("Back to Dashboard").set("href", "dashboard.cgi") << " | ";
    cout << a("Logout").set("href", "logout.cgi") << endl;
    cout << cgicc::div() << h1("User Management") << endl;
    cout << cgicc::div().set("class", "form-box") << endl;
    cout << h2("Create New User") << endl;
    cout << form().set("method", "post").set("action", "admin.cgi") << endl;
    cout << "Username: " << input().set("type", "text").set("name", "username").set("required", "true") << br();
    cout << "Password: " << input().set("type", "text").set("name", "password").set("required", "true") << br();
    cout << "Email: " << input().set("type", "email").set("name", "email").set("required", "true") << br();
    cout << "Role: ";
    cout << cgicc::select().set("name", "role") << endl;
    cout << option("User").set("value", "user") << endl;
    cout << option("Admin").set("value", "admin") << endl;
    cout << cgicc::select() << br() << br();
    cout << input().set("type", "submit").set("value", "Create User") << endl;
    cout << form() << cgicc::div() << endl;
    cout << h2("Existing Users") << endl;
    cout << "<table><tr><th>ID</th><th>Username</th><th>Email</th><th>Role</th></tr>";
    
    MYSQL_RES* res = db.select("SELECT id, username, email, role FROM users");
    if (res) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            cout << "<tr>";
            cout << "<td>" << row[0] << "</td>";
            cout << "<td>" << row[1] << "</td>";
            cout << "<td>" << row[2] << "</td>";
            cout << "<td>" << row[3] << "</td>";
            cout << "</tr>";
        }
    }
    cout << "</table>";

    cout << body() << html() << endl;
    return 0;
}