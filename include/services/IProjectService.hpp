#pragma once

#include "../models/Project.hpp"
#include <string>
#include <optional>
#include <vector>


class IProjectService
{
public:
    virtual ~IProjectService() = default;

    virtual Project create_project(const std::string& name, const std::string& description, const std::string& start_date, const std::string& end_date) = 0;
    virtual std::optional<Project> get_project(int id) = 0;
    virtual std::vector<Project> get_all_projects() = 0;
    virtual void update_project(int id, const std::string& name, const std::string& description, const std::string& start_date, const std::string& end_date) = 0;
    virtual void delete_project(int id) = 0;
    virtual void delete_all_projects() = 0;
};