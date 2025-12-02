#include <iostream>
#include <string>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTTPRedirectHeader.h>
#include <cgicc/HTMLClasses.h>
#include "include/Database.h"
#include "include/SessionManager.h"
#include "include/Post.h"

using namespace std;
using namespace cgicc;

int main() {
    Cgicc cgi;
    Database db;
    db.connect();
    SessionManager session(&db);
    Post postManager(&db);

    // SR4: Security Check
    int userId = session.checkSession(cgi);
    if (userId == 0) {
        cout << HTTPRedirectHeader("login.cgi") << endl;
        return 0;
    }

    // FR1: Handle Form Submission
    const_form_iterator titleIter = cgi.getElement("title");
    const_form_iterator contentIter = cgi.getElement("content");

    if (titleIter != cgi.getElements().end() && contentIter != cgi.getElements().end()) {
        string title = titleIter->getValue();
        string content = contentIter->getValue();

        if (postManager.createPost(userId, title, content)) {
            // Success: Redirect to Dashboard
            cout << HTTPRedirectHeader("dashboard.cgi") << endl;
            return 0;
        }
    }

    // Render Form
    cout << HTTPHTMLHeader() << endl;
    cout << html() << head(title("New Post")) << endl;
    cout << style("body { font-family: sans-serif; max-width: 600px; margin: 40px auto; padding: 20px; }"
                  "input[type=text], textarea { width: 100%; padding: 10px; margin: 10px 0; display: block; }"
                  "textarea { height: 150px; font-family: sans-serif; }" 
                  "input[type=submit] { padding: 10px 20px; background: #333; color: white; border: none; cursor: pointer; }") << endl;
    cout << body() << endl;

    cout << h1("Write a New Blog Post") << endl;
    
    cout << form().set("method", "post").set("action", "new_post.cgi") << endl;
    
    cout << b("Title:") << br();
    cout << input().set("type", "text").set("name", "title").set("required", "true") << endl;
    
    cout << b("Content:") << br();
    // FIX: textarea("") ensures the tag is closed properly: <textarea>...</textarea>
    cout << textarea("").set("name", "content").set("required", "true") << endl; 
    
    cout << input().set("type", "submit").set("value", "Publish Post") << endl;
    
    cout << form() << endl;
    cout << cgicc::p(a("Back to Dashboard").set("href", "dashboard.cgi")) << endl;

    cout << body() << html() << endl;
    return 0;
}