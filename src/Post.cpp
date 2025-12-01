#include "../include/Post.h"
#include <iostream>

Post::Post(Database* database) {
    this->db = database;
}

Post::~Post() {}

vector<BlogPost> Post::getAllPosts() {
    vector<BlogPost> posts;
    
    // Join with users table to get the username instead of just user_id
    string query = "SELECT p.id, p.title, p.content, p.rating, u.username "
                   "FROM posts p "
                   "JOIN users u ON p.user_id = u.id "
                   "ORDER BY p.created_at DESC";

    MYSQL_RES* res = db->select(query);
    if (!res) return posts;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        BlogPost p;
        p.id = stoi(row[0]);
        p.title = row[1];
        p.content = row[2];
        p.rating = stoi(row[3]);
        p.author = row[4];
        posts.push_back(p);
    }
    return posts;
}

void Post::ratePost(int postId, int change) {
    // Basic validation to ensure change is only 1 or -1
    if (change > 1) change = 1;
    if (change < -1) change = -1;

    string query = "UPDATE posts SET rating = rating + (" + to_string(change) + ") WHERE id = " + to_string(postId);
    db->execute(query);
}