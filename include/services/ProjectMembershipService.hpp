#pragma once

#include "IProjectMembershipService.hpp"
#include "../repositories/IProjectMembershipRepository.hpp"
#include <memory>


class ProjectMembershipService : public IProjectMembershipService
{
private:
    std::shared_ptr<IProjectMembershipRepository> repository;

public:
    explicit ProjectMembershipService(std::shared_ptr<IProjectMembershipRepository> repo);
    ~ProjectMembershipService() override = default;

    void add_user_to_project(int user_id, int project_id) override;
    void remove_user_from_project(int user_id, int project_id) override;
    std::vector<User> get_project_users(int project_id) override;
    std::vector<Project> get_user_projects(int user_id) override;
};