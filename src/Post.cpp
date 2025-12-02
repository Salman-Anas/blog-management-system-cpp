#include "../include/Post.h"
#include <iostream>

Post::Post(Database* database) {
    this->db = database;
}

Post::~Post() {}

vector<BlogPost> Post::getAllPosts() {
    vector<BlogPost> posts;
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
    if (change > 1) change = 1;
    if (change < -1) change = -1;

    string query = "UPDATE posts SET rating = rating + (" + to_string(change) + ") WHERE id = " + to_string(postId);
    db->execute(query);
}


bool Post::createPost(int userId, string title, string content) {
    string cleanTitle = db->sanitize(title);
    string cleanContent = db->sanitize(content);

    string query = "INSERT INTO posts (user_id, title, content) VALUES (" 
                   + to_string(userId) + ", '" + cleanTitle + "', '" + cleanContent + "')";
    
    return db->execute(query);
}

BlogPost Post::getPostById(int postId) {
    BlogPost p;
    p.id = 0; 

    string query = "SELECT id, title, content, user_id FROM posts WHERE id = " + to_string(postId);
    MYSQL_RES* res = db->select(query);
    if (!res) return p;

    MYSQL_ROW row = mysql_fetch_row(res);
    if (row) {
        p.id = stoi(row[0]);
        p.title = row[1];
        p.content = row[2];
        p.rating = stoi(row[3]); 
    }
    return p;
}

bool Post::updatePost(int postId, int userId, string title, string content) {
    string cleanTitle = db->sanitize(title);
    string cleanContent = db->sanitize(content);
    string query = "UPDATE posts SET title = '" + cleanTitle + "', content = '" + cleanContent + 
                   "' WHERE id = " + to_string(postId) + " AND user_id = " + to_string(userId);
    
    return db->execute(query);
}