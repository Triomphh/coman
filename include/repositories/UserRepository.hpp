#pragma once

#include "IUserRepository.hpp"
#include <SQLiteCpp/SQLiteCpp.h>


class UserRepository : public IUserRepository 
{
private:
    std::shared_ptr<SQLite::Database> db;

public:
    UserRepository(std::shared_ptr<SQLite::Database> database) : db(std::move(database)) {}

    void create(const User& user) override;
    std::optional<User> get(int id) override;
    std::optional<User> get_by_email(const std::string& email) override;
    std::vector<User> get_all() override;
    void update(const User& user) override;
    void remove(int id) override;
    void remove_all() override;
};