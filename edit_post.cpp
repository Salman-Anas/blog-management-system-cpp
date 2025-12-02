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

    int userId = session.checkSession(cgi);
    if (userId == 0) {
        cout << HTTPRedirectHeader("login.cgi") << endl;
        return 0;
    }
    const_form_iterator titleIter = cgi.getElement("title");
    if (titleIter != cgi.getElements().end()) {
        int postId = stoi(cgi.getElement("post_id")->getValue());
        string title = titleIter->getValue();
        string content = cgi.getElement("content")->getValue();
        if (postManager.updatePost(postId, userId, title, content)) {
            cout << HTTPRedirectHeader("dashboard.cgi") << endl;
        } else {
            cout << HTTPHTMLHeader() << endl;
            cout << "Error: You cannot edit this post." << endl;
        }
        return 0;
    }
    const_form_iterator idIter = cgi.getElement("id");
    if (idIter == cgi.getElements().end()) {
        cout << HTTPRedirectHeader("dashboard.cgi") << endl;
        return 0;
    }

    int postId = stoi(idIter->getValue());
    BlogPost post = postManager.getPostById(postId);

    if (post.id == 0 || post.rating != userId) {
        cout << HTTPRedirectHeader("dashboard.cgi") << endl;
        return 0;
    }

    cout << HTTPHTMLHeader() << endl;
    cout << html() << head(title("Edit Post")) << endl;
    cout << style("body { font-family: sans-serif; max-width: 600px; margin: 40px auto; padding: 20px; }"
                  "input[type=text], textarea { width: 100%; padding: 10px; margin: 10px 0; display: block; }"
                  "textarea { height: 150px; font-family: sans-serif; }" 
                  "input[type=submit] { padding: 10px 20px; background: #333; color: white; border: none; cursor: pointer; }") << endl;
    cout << body() << endl;
    cout << h1("Edit Blog Post") << endl;
    cout << form().set("method", "post").set("action", "edit_post.cgi") << endl;
    cout << input().set("type", "hidden").set("name", "post_id").set("value", to_string(postId)) << endl;
    cout << b("Title:") << br();
    cout << input().set("type", "text").set("name", "title").set("value", post.title).set("required", "true") << endl;
    cout << b("Content:") << br();
    cout << textarea(post.content).set("name", "content").set("required", "true") << endl;
    cout << input().set("type", "submit").set("value", "Save Changes") << endl;
    cout << form() << endl;
    cout << cgicc::p(a("Cancel").set("href", "dashboard.cgi")) << endl;
    cout << body() << html() << endl;
    return 0;
}