#pragma once

#include <string>
#include <vector>

class Project 
{
public:
    int id;
    std::string name;
    std::string description;
};

class ProjectRepository 
{
public:
    static void create_project(const std::string& name, const std::string& description);
    static std::vector<Project> get_projects();
    static void update_project(int id, const std::string& name, const std::string& description);
    static void delete_project(int id);
    static void delete_all_projects();
}; 