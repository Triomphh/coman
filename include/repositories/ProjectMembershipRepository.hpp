#pragma once

#include "IProjectMembershipRepository.hpp"
#include <SQLiteCpp/SQLiteCpp.h>


class ProjectMembershipRepository : public IProjectMembershipRepository
{
private:
    std::shared_ptr<SQLite::Database> db;

public:
    ProjectMembershipRepository(std::shared_ptr<SQLite::Database> database) : db(std::move(database)) {}

    void add_user(int user_id, int project_id) override;
    void remove_user(int user_id, int project_id) override;
    std::vector<User> get_project_users(int project_id) override;
    std::vector<Project> get_user_projects(int user_id) override;
};