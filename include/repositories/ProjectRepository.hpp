#pragma once

#include "IProjectRepository.hpp"
#include <SQLiteCpp/SQLiteCpp.h>


class ProjectRepository : public IProjectRepository 
{
private:
    std::shared_ptr<SQLite::Database> db;

public:
    ProjectRepository(std::shared_ptr<SQLite::Database> database) : db(std::move(database)) {}

    void create(const Project& project) override;
    std::optional<Project> get(int id) override;
    std::vector<Project> get_all() override;
    void update(const Project& project) override;
    void remove(int id) override;
    void remove_all() override;
};