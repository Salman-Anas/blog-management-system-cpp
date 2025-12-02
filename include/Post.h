#ifndef POST_H
#define POST_H

#include <string>
#include <vector>
#include "Database.h"

using namespace std;

// A simple structure to hold data for a single post
struct BlogPost {
    int id;
    string title;
    string content;
    string author;
    int rating;
};

class Post {
private:
    Database* db;

public:
    Post(Database* database);
    ~Post();

    // FR2: Fetch all posts to view them
    vector<BlogPost> getAllPosts();

    // FR2: Uprate or Downrate a post
    void ratePost(int postId, int change); // change is +1 or -1

    bool createPost(int userId, string title, string content);
};

#endif