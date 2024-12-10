#pragma once

#include "IAuthenticationService.hpp"
#include "../repositories/IUserRepository.hpp"
#include <memory>


class AuthenticationService : public IAuthenticationService
{
private:
    std::shared_ptr<IUserRepository> user_repository;
    // later add other dependencies, like token based authentication ? 

public:
    explicit AuthenticationService(std::shared_ptr<IUserRepository> user_repository);

    std::optional<User> authenticate(const std::string& email, const std::string& password) override;
    bool verify_password(const std::string& password, const std::string& hash) override;
};