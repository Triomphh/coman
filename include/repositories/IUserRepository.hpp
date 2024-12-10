#pragma once

#include "../models/User.hpp"
#include <optional>
#include <vector>


class IUserRepository
{
public:
    virtual ~IUserRepository() = default;

    // CRUD operations
    virtual void create(const User& user) = 0;
    virtual std::optional<User> get(int id) = 0;
    virtual std::optional<User> get_by_email(const std::string& email) = 0;
    virtual std::vector<User> get_all() = 0;
    virtual void update(const User& user) = 0;
    virtual void remove(int id) = 0;
    virtual void remove_all() = 0;
};