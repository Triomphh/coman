#include "User.hpp"


bool User::hasPermission(UserRole minimum_role) const 
{
    return static_cast<int>(role) >= static_cast<int>(minimum_role);
}