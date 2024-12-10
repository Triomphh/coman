#include "../../include/services/UserService.hpp"


UserService::UserService(std::shared_ptr<IUserRepository> repo) : repository(std::move(repo)) {}



User UserService::create_user(const std::string& name, const std::string& email, const std::string& password, UserRole role, const std::string& profile_picture)
{
    // TODO: validate input

    User user;
    user.name            = name;
    user.email           = email;
    user.hashed_password = password; // To change
    user.role            = role;
    user.profile_picture = profile_picture.empty() ? "default.png" : profile_picture;

    repository->create(user);
    
    return user;
}


std::optional<User> UserService::get_user(int id)
{
    // TODO: validate input

    return repository->get(id);
}


std::optional<User> UserService::get_user_by_email(const std::string& email)
{
    // TODO: validate input

    return repository->get_by_email(email);
}


std::vector<User> UserService::get_all_users()
{
    return repository->get_all();
}


void UserService::update_user(int id, const std::string& name, const std::string& email, const std::string& password)
{
    // TODO: validate input

    User user;
    user.id              = id;
    user.name            = name;
    user.email           = email;
    user.hashed_password = password; // To change

    repository->update(user);
}


void UserService::delete_user(int id)
{
    // TODO: validate input

    repository->remove(id);
}


void UserService::delete_all_users()
{
    repository->remove_all();
}