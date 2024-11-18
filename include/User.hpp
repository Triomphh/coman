#pragma once

#include <string>

// Define user roles
enum class UserRole     // the lower the number, the higher the access level
{
    TEAM_MEMBER,        // Basic access level
    PROJECT_MANAGER,    // Can manage projects and team members
    ADMIN               // Full system access
};

class User 
{
public:
    int id;
    std::string name;
    std::string email;
    std::string hashed_password;
    UserRole role;

    // Helper method to check permissions
    bool hasPermission(UserRole minimum_role) const 
    {
        return static_cast<int>(role) >= static_cast<int>(minimum_role);
    }
}; 