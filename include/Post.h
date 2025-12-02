#ifndef POST_H
#define POST_H

#include <string>
#include <vector>
#include "Database.h"

using namespace std;
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
    vector<BlogPost> getAllPosts();
    void ratePost(int postId, int change);
    bool createPost(int userId, string title, string content);
    BlogPost getPostById(int postId);
    bool updatePost(int postId, int userId, string title, string content);
};

#endif