#pragma once

#include "../models/User.hpp"
#include <string>
#include <optional>
#include <vector>


class IUserService
{
public:
    virtual ~IUserService() = default;

    virtual User create_user(const std::string& name, const std::string& email, const std::string& password, UserRole role, const std::string& profile_picture) = 0;
    virtual std::optional<User> get_user(int id) = 0;
    virtual std::optional<User> get_user_by_email(const std::string& email) = 0;
    virtual std::vector<User> get_all_users() = 0;
    virtual void update_user(int id, const std::string& name, const std::string& email, const std::string& password) = 0;
    virtual void delete_user(int id) = 0;
    virtual void delete_all_users() = 0;
};