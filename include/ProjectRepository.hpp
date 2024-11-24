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

    // Manage project members
    static void add_user_to_project(int user_id, int project_id);
    static void remove_user_from_project(int user_id, int project_id);
    static std::vector<User> get_project_users(int project_id);
    static std::vector<Project> get_user_projects(int user_id);
};