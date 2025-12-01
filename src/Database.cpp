#include "../include/Database.h"
#include <iostream>
#include <cstring> // For strlen

Database::Database() {
    conn = mysql_init(NULL);
    if (conn == NULL) {
        std::cerr << "mysql_init() failed\n";
    }
}

Database::~Database() {
    if (conn) {
        mysql_close(conn);
    }
}

bool Database::connect() {
    // Connect to the database using the credentials from the header
    if (mysql_real_connect(conn, db_host.c_str(), db_user.c_str(), 
                           db_pass.c_str(), db_name.c_str(), 0, NULL, 0) == NULL) {
        std::cerr << "mysql_real_connect() failed\n";
        std::cerr << mysql_error(conn) << "\n";
        return false;
    }
    return true;
}

bool Database::execute(string query) {
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "Query Error: " << mysql_error(conn) << "\n";
        return false;
    }
    return true;
}

MYSQL_RES* Database::select(string query) {
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "Query Error: " << mysql_error(conn) << "\n";
        return NULL;
    }
    return mysql_store_result(conn);
}

// SR6: Sanitize inputs to prevent SQL Injection
string Database::sanitize(string input) {
    char* escaped = new char[input.length() * 2 + 1];
    mysql_real_escape_string(conn, escaped, input.c_str(), input.length());
    string clean_input = escaped;
    delete[] escaped;
    return clean_input;
}