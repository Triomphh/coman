#pragma once

#include "../models/Project.hpp"
#include "../models/User.hpp"

class IProjectMembershipService
{
public:
    virtual ~IProjectMembershipService() = default;

    virtual void add_user_to_project(int user_id, int project_id) = 0;
    virtual void remove_user_from_project(int user_id, int project_id) = 0;
    virtual std::vector<User> get_project_users(int project_id) = 0;
    virtual std::vector<Project> get_user_projects(int user_id) = 0;
};