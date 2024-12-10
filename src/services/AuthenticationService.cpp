#include "../../include/services/AuthenticationService.hpp"


AuthenticationService::AuthenticationService(std::shared_ptr<IUserRepository> user_repository) : user_repository(std::move(user_repository)) {}


std::optional<User> AuthenticationService::authenticate(const std::string& email, const std::string& password)
{
    auto user = user_repository->get_by_email(email);
    if (!user)
        return std::nullopt;

    if (!verify_password(password, user->hashed_password))
        return std::nullopt;

    // clear hashed password from user object ?
    // user->hashed_password.clear();
    return user;
}


bool AuthenticationService::verify_password(const std::string& password, const std::string& hash)
{
    return password == hash;
}