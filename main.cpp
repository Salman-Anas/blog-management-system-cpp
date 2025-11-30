#include <iostream>
#include <vector>
#include <string>

// CGICC Libraries (Standard C++ Web Libraries)
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>

using namespace std;
using namespace cgicc;

int main() {
    try {
        Cgicc cgi;

        // Send HTTP Header
        cout << HTTPHTMLHeader() << endl;

        // Start HTML Page
        cout << html() << head(title("Blog System Test")) << endl;
        cout << body() << endl;
        cout << h1("Environment Check") << endl;
        cout << "C++ CGI is working!" << br() << endl;
        
        cout << body() << html() << endl;
    }
    catch(exception& e) {
        // Handle errors
        cout << "Error: " << e.what() << endl;
    }
    return 0;
}
