#include "../include/UserRepository.hpp"
#include <SQLiteCpp/SQLiteCpp.h>


void UserRepository::create_user(const std::string& name, const std::string& email, const std::string& password, UserRole role) 
{
    SQLite::Database db("database.db", SQLite::OPEN_READWRITE);
    SQLite::Statement query(db, "INSERT INTO users (name, email, hashed_password, role) VALUES (?, ?, ?, ?)");
    query.bind(1, name);
    query.bind(2, email);
    query.bind(3, password);
    query.bind(4, static_cast<int>(role));
    query.exec();
}

std::optional<User> UserRepository::get_user(int id) 
{
    SQLite::Database db("database.db", SQLite::OPEN_READONLY);
    SQLite::Statement query(db, "SELECT id, name, email, hashed_password, role FROM users WHERE id = ?");
    query.bind(1, id);

    if (query.executeStep())
    {
        User user;
        user.id              = query.getColumn(0);
        user.name            = query.getColumn(1).getText();
        user.email           = query.getColumn(2).getText();
        user.hashed_password = query.getColumn(3).getText();
        user.role            = static_cast<UserRole>(query.getColumn(4).getInt());
        return user;
    }

    return std::nullopt;
}

std::vector<User> UserRepository::get_users() 
{
    std::vector<User> users;
    SQLite::Database db("database.db", SQLite::OPEN_READONLY);
    SQLite::Statement query(db, "SELECT id, name, email, hashed_password, role FROM users");

    while (query.executeStep()) 
    {
        User user;
        user.id = query.getColumn(0);
        user.name = query.getColumn(1).getText();
        user.email = query.getColumn(2).getText();
        user.hashed_password = query.getColumn(3).getText();
        user.role = static_cast<UserRole>(query.getColumn(4).getInt());
        users.push_back(user);
    }

    return users;
}

void UserRepository::delete_user(int id) 
{
    SQLite::Database db("database.db", SQLite::OPEN_READWRITE);
    SQLite::Statement query(db, "DELETE FROM users WHERE id = ?");
    query.bind(1, id);
    query.exec();
}

void UserRepository::delete_all_users() 
{
    SQLite::Database db("database.db", SQLite::OPEN_READWRITE);
    db.exec("DELETE FROM users");
}