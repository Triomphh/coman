#pragma once

#include "User.hpp"
#include <optional>
#include <string>
#include <vector>


class UserRepository 
{
public:
    // CRUD operations
    static void create_user(const std::string& name, const std::string& email, const std::string& password, UserRole role);
    static std::optional<User> get_user(int id);
    static std::optional<User> get_user_by_email(const std::string& email);
    static std::vector<User> get_users();
    static void update_user(int id, const std::string& name, const std::string& email, const std::string& password, UserRole role);
    static void delete_user(int id);
    static void delete_all_users();

    static std::optional<User> authenticate(const std::string& username, const std::string& password);


private:
    static bool verify_password(const std::string& password, const std::string& hash);
}; 