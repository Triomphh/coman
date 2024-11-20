#include "../include/ProjectRepository.hpp"
#include <SQLiteCpp/SQLiteCpp.h>

std::vector<Project> ProjectRepository::get_projects() 
{
    std::vector<Project> projects;
    SQLite::Database db("database.db", SQLite::OPEN_READONLY);
    SQLite::Statement query(db, "SELECT id, name, description, start_date, end_date FROM projects");
    
    while (query.executeStep()) 
    {
        Project project;
        project.id = query.getColumn(0);
        project.name = query.getColumn(1).getText();
        project.description = query.getColumn(2).getText();
        project.start_date = query.getColumn(3).getText();
        project.end_date = query.getColumn(4).getText();
        projects.push_back(project);
    }
    
    return projects;
}

std::optional<Project> ProjectRepository::get_project(int id) 
{
    SQLite::Database db("database.db", SQLite::OPEN_READONLY);
    SQLite::Statement query(db, "SELECT id, name, description, start_date, end_date FROM projects WHERE id = ?");
    query.bind(1, id);
    
    if (query.executeStep()) {
        Project project;
        project.id = query.getColumn(0);
        project.name = query.getColumn(1).getText();
        project.description = query.getColumn(2).getText();
        project.start_date = query.getColumn(3).getText();
        project.end_date = query.getColumn(4).getText();
        return project;
    }
    
    return std::nullopt;
}

void ProjectRepository::create_project(const std::string& name, const std::string& description, const std::string& start_date, const std::string& end_date) 
{
    SQLite::Database db("database.db", SQLite::OPEN_READWRITE);
    SQLite::Statement query(db, "INSERT INTO projects (name, description, start_date, end_date) VALUES (?, ?, ?, ?)");
    query.bind(1, name);
    query.bind(2, description);
    query.bind(3, start_date);
    query.bind(4, end_date);
    query.exec();
}

void ProjectRepository::delete_project(int id) 
{
    SQLite::Database db("database.db", SQLite::OPEN_READWRITE);
    SQLite::Statement query(db, "DELETE FROM projects WHERE id = ?");
    query.bind(1, id);
    query.exec();
}

void ProjectRepository::delete_all_projects() 
{
    SQLite::Database db("database.db", SQLite::OPEN_READWRITE);
    db.exec("DELETE FROM projects");
}