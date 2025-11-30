#ifndef DATABASE_H
#define DATABASE_H

#include <mysql/mysql.h>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Database {
private:
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    string db_host = "localhost";
    string db_user = "blog_admin";
    string db_pass = "secure_password";
    string db_name = "blog_system";

public:
    Database();
    ~Database();
    bool connect();

    bool execute(string query);
    MYSQL_RES* select(string query);
    string sanitize(string input);
};
#endif
