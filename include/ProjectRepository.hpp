#pragma once

#include "Project.hpp"
#include "User.hpp"
#include <string>
#include <vector>
#include <optional>


class ProjectRepository 
{
public:
    // CRUD operations
    static void create_project(const std::string& name, const std::string& description, const std::string& start_date, const std::string& end_date);
    static std::optional<Project> get_project(int id);
    static std::vector<Project> get_projects();
    static void update_project(int id, const std::string& name, const std::string& description, const std::string& start_date, const std::string& end_date);
    static void delete_project(int id);
    static void delete_all_projects();

};