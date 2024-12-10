#include "../../include/repositories/UserRepository.hpp"


void UserRepository::create(const User& user)
{
    SQLite::Statement query(*db, "INSERT INTO users (name, email, hashed_password, role, profile_picture) VALUES (?, ?, ?, ?, ?)");
    query.bind(1, user.name);
    query.bind(2, user.email);
    query.bind(3, user.hashed_password);
    query.bind(4, static_cast<int>(user.role));
    query.bind(5, user.profile_picture);
    query.exec();
}

std::optional<User> UserRepository::get(int id)
{
    SQLite::Statement query(*db, "SELECT id, name, email, hashed_password, role, profile_picture FROM users WHERE id = ?");
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

std::optional<User> UserRepository::get_by_email(const std::string& email)
{
    SQLite::Statement query(*db, "SELECT id, name, email, hashed_password, role, profile_picture FROM users WHERE email = ?");
    query.bind(1, email);

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

std::vector<User> UserRepository::get_all()
{
    std::vector<User> users;
    SQLite::Statement query(*db, "SELECT id, name, email, hashed_password, role, profile_picture FROM users");

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

void UserRepository::update(const User& user)
{
    SQLite::Statement query(*db, "UPDATE users SET name = ?, email = ?, hashed_password = ?, role = ?, profile_picture = ? WHERE id = ?");
    query.bind(1, user.name);
    query.bind(2, user.email);
    query.bind(3, user.hashed_password);
    query.bind(4, static_cast<int>(user.role));
    query.bind(5, user.profile_picture);
    query.bind(6, user.id);
    query.exec();
}

void UserRepository::remove(int id)
{
    SQLite::Statement query(*db, "DELETE FROM users WHERE id = ?");
    query.bind(1, id);
    query.exec();
}

void UserRepository::remove_all()
{
    SQLite::Statement query(*db, "DELETE FROM users");
    query.exec();
}