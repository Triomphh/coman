#pragma once

#include "IUserService.hpp"
#include "../repositories/IUserRepository.hpp"
#include <memory>


class UserService : public IUserService
{
private:
    std::shared_ptr<IUserRepository> repository;

public:
    explicit UserService(std::shared_ptr<IUserRepository> repo);
    ~UserService() override = default;

    User create_user(const std::string& name, const std::string& email, const std::string& password, UserRole role, const std::string& profile_picture) override;
    std::optional<User> get_user(int id) override;
    std::optional<User> get_user_by_email(const std::string& email) override;
    std::vector<User> get_all_users() override;
    void update_user(int id, const std::string& name, const std::string& email, const std::string& password) override;
    void delete_user(int id) override;
    void delete_all_users() override;
};