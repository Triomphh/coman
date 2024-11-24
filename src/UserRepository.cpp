#include "../include/UserRepository.hpp"
#include <SQLiteCpp/SQLiteCpp.h>


void UserRepository::create_user(const std::string& name, const std::string& email, const std::string& password, UserRole role, const std::string& profile_picture) 
{
    SQLite::Database db("database.db", SQLite::OPEN_READWRITE);
    SQLite::Statement query(db, "INSERT INTO users (name, email, hashed_password, role, profile_picture) VALUES (?, ?, ?, ?, ?)");
    query.bind(1, name);
    query.bind(2, email);
    query.bind(3, password);
    query.bind(4, static_cast<int>(role));
    query.bind(5, profile_picture);
    query.exec();
}

std::optional<User> UserRepository::get_user(int id) 
{
    SQLite::Database db("database.db", SQLite::OPEN_READONLY);
    SQLite::Statement query(db, "SELECT id, name, email, hashed_password, role, profile_picture FROM users WHERE id = ?");
    query.bind(1, id);

    if (query.executeStep())
    {
        User user;
        user.id              = query.getColumn(0);
        user.name            = query.getColumn(1).getText();
        user.email           = query.getColumn(2).getText();
        user.hashed_password = query.getColumn(3).getText();
        user.role            = static_cast<UserRole>(query.getColumn(4).getInt());
        user.profile_picture = query.getColumn(5).getText();
        return user;
    }

    return std::nullopt;
}

std::optional<User> UserRepository::get_user_by_email(const std::string& email) 
{
    SQLite::Database db("database.db", SQLite::OPEN_READONLY);
    SQLite::Statement query(db, "SELECT id, name, email, hashed_password, role, profile_picture FROM users WHERE email = ?");
    query.bind(1, email);

    if (query.executeStep())
    {
        User user;
        user.id = query.getColumn(0);
        user.name = query.getColumn(1).getText();
        user.email = query.getColumn(2).getText();
        user.hashed_password = query.getColumn(3).getText();
        user.role = static_cast<UserRole>(query.getColumn(4).getInt());
        user.profile_picture = query.getColumn(5).getText();
        return user;
    }

    return std::nullopt;
}

std::vector<User> UserRepository::get_users() 
{
    std::vector<User> users;
    SQLite::Database db("database.db", SQLite::OPEN_READONLY);
    SQLite::Statement query(db, "SELECT id, name, email, hashed_password, role, profile_picture FROM users");

    while (query.executeStep()) 
    {
        User user;
        user.id              = query.getColumn(0);
        user.name            = query.getColumn(1).getText();
        user.email           = query.getColumn(2).getText();
        user.hashed_password = query.getColumn(3).getText();
        user.role            = static_cast<UserRole>(query.getColumn(4).getInt());
        user.profile_picture = query.getColumn(5).getText();
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

// Add this method to your UserRepository class
std::optional<User> UserRepository::authenticate(const std::string& email, const std::string& password) 
{
    SQLite::Database db("database.db", SQLite::OPEN_READONLY);
    SQLite::Statement query(db, "SELECT * FROM users WHERE email = ?");
    query.bind(1, email);

    if (query.executeStep()) 
    {
        std::string stored_hash = query.getColumn("hashed_password").getText();
        
        if (UserRepository::verify_password(password, stored_hash)) 
        {
            User user;
            user.id              = query.getColumn("id").getInt();
            user.name            = query.getColumn("name").getText();
            user.email           = query.getColumn("email").getText();
            user.role            = static_cast<UserRole>(query.getColumn("role").getInt());
            user.profile_picture = query.getColumn("profile_picture").getText();
            return user;
        }
    }

    return std::nullopt;
}

// For now, simple comparison
bool UserRepository::verify_password(const std::string& password, const std::string& hash) 
{
    return password == hash;
}