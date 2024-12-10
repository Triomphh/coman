#pragma once

#include "../models/User.hpp"
#include <optional>


class IAuthenticationService
{
public:
    virtual ~IAuthenticationService() = default;

    virtual std::optional<User> authenticate(const std::string& email, const std::string& password) = 0;
    virtual bool verify_password(const std::string& password, const std::string& hash) = 0;
};