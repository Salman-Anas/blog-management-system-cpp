#include <iostream>
#include <vector>
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

    // FR2: Handle Rating Actions
    const_form_iterator actionIter = cgi.getElement("action");
    if (actionIter != cgi.getElements().end()) {
        int postId = stoi(cgi.getElement("post_id")->getValue());
        string action = actionIter->getValue();
        
        if (action == "up") postManager.ratePost(postId, 1);
        if (action == "down") postManager.ratePost(postId, -1);

        cout << HTTPRedirectHeader("dashboard.cgi") << endl;
        return 0;
    }

    // Render Dashboard
    cout << HTTPHTMLHeader() << endl;
    cout << html() << head(title("Dashboard")) << endl;
    
    cout << style("body { font-family: sans-serif; max-width: 800px; margin: 0 auto; padding: 20px; }"
                  ".post { border: 1px solid #ddd; padding: 15px; margin-bottom: 20px; border-radius: 5px; }"
                  ".rating { float: right; font-weight: bold; font-size: 1.2em; }"
                  ".btn { padding: 5px 10px; cursor: pointer; }"
                  ".nav { background: #333; color: white; padding: 10px; margin-bottom: 20px; }"
                  ".nav a { color: white; margin-right: 15px; text-decoration: none; }") << endl;

    cout << body() << endl;

    // FIX: Use cgicc::div explicitly to avoid ambiguity with math div()
    cout << cgicc::div().set("class", "nav") << endl;
    cout << span("Logged in as User ID: " + to_string(userId)) << " | ";
    cout << a("New Post").set("href", "new_post.cgi") << " | ";
    cout << a("Logout").set("href", "logout.cgi") << endl;
    cout << cgicc::div() << endl;

    cout << h1("Recent Blog Posts") << endl;

    vector<BlogPost> posts = postManager.getAllPosts();
    
    if (posts.empty()) {
        cout << cgicc::p("No posts found. Be the first to write one!") << endl;
    } else {
        // FIX: Renamed loop variable 'p' to 'post' to avoid conflict with HTML p() tag
        for (const auto& post : posts) {
            cout << cgicc::div().set("class", "post") << endl;
            
            // Rating Section
            cout << cgicc::div().set("class", "rating") << endl;
            cout << "Rating: " << post.rating << br();
            
            cout << form().set("method", "post").set("action", "dashboard.cgi").set("style", "display:inline;") << endl;
            cout << input().set("type", "hidden").set("name", "post_id").set("value", to_string(post.id));
            cout << input().set("type", "submit").set("name", "action").set("value", "up").set("class", "btn");
            cout << form() << endl;

            cout << form().set("method", "post").set("action", "dashboard.cgi").set("style", "display:inline;") << endl;
            cout << input().set("type", "hidden").set("name", "post_id").set("value", to_string(post.id));
            cout << input().set("type", "submit").set("name", "action").set("value", "down").set("class", "btn");
            cout << form() << endl;
            cout << cgicc::div() << endl;

            // Content Section
            cout << h2(post.title) << endl;
            cout << cgicc::p("By: " + post.author) << endl;
            cout << cgicc::p(post.content) << endl;
            
            cout << cgicc::div() << endl;
        }
    }

    cout << body() << html() << endl;
    return 0;
}