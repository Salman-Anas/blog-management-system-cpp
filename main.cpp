#include <iostream>
#include <vector>
#include <string>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>
#include "include/Database.h" // Include your new class

using namespace std;
using namespace cgicc;

int main() {
    // 1. Always print the header first or the browser will show a 500 error
    cout << HTTPHTMLHeader() << endl;
    cout << html() << head(title("Database Test")) << body() << endl;

    // 2. Test the connection
    Database db;
    cout << h1("System Diagnostics") << endl;
    
    if (db.connect()) {
        cout << p("Database Connection: ") << b("SUCCESS") << style("color:green") << endl;
        
        // Optional: Test a quick query
        MYSQL_RES* res = db.select("SELECT DATABASE()");
        if (res) {
             MYSQL_ROW row = mysql_fetch_row(res);
             cout << p("Connected to schema: " + string(row[0])) << endl;
        }
    } else {
        cout << p("Database Connection: ") << b("FAILED") << style("color:red") << endl;
        cout << p("Check your username/password in Database.h") << endl;
    }

    cout << body() << html() << endl;
    return 0;
}