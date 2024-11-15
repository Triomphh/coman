#include "../include/project_repository.hpp"
#include <SQLiteCpp/SQLiteCpp.h>

std::vector<Project> ProjectRepository::get_projects() 
{
    std::vector<Project> projects;
    SQLite::Database db("database.db", SQLite::OPEN_READONLY);
    SQLite::Statement query(db, "SELECT id, name, description FROM projects");
    
    while (query.executeStep()) 
    {
        Project project;
        project.id = query.getColumn(0);
        project.name = query.getColumn(1).getText();
        project.description = query.getColumn(2).getText();
        projects.push_back(project);
    }
    
    return projects;
}

void ProjectRepository::create_project(const std::string& name, const std::string& description) 
{
    SQLite::Database db("database.db", SQLite::OPEN_READWRITE);
    SQLite::Statement query(db, "INSERT INTO projects (name, description) VALUES (?, ?)");
    query.bind(1, name);
    query.bind(2, description);
    query.exec();
}

void ProjectRepository::delete_all_projects() 
{
    SQLite::Database db("database.db", SQLite::OPEN_READWRITE);
    db.exec("DELETE FROM projects");
} 