#include "../include/ProjectRepository.hpp"
#include <SQLiteCpp/SQLiteCpp.h>



void ProjectRepository::create_project(const std::string& name, const std::string& description, const std::string& start_date, const std::string& end_date) 
{
    SQLite::Database db("data/database.db", SQLite::OPEN_READWRITE);
    SQLite::Statement query(db, "INSERT INTO projects (name, description, start_date, end_date) VALUES (?, ?, ?, ?)");
    query.bind(1, name);
    query.bind(2, description);
    query.bind(3, start_date);
    query.bind(4, end_date);
    query.exec();
}

std::vector<Project> ProjectRepository::get_projects() 
{
    std::vector<Project> projects;
    SQLite::Database db("data/database.db", SQLite::OPEN_READONLY);
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
    SQLite::Database db("data/database.db", SQLite::OPEN_READONLY);
    SQLite::Statement query(db, "SELECT id, name, description, start_date, end_date FROM projects WHERE id = ?");
    query.bind(1, id);
    
    if (query.executeStep()) {
        Project project;
        project.id          = query.getColumn(0);
        project.name        = query.getColumn(1).getText();
        project.description = query.getColumn(2).getText();
        project.start_date  = query.getColumn(3).getText();
        project.end_date    = query.getColumn(4).getText();
        return project;
    }
    
    return std::nullopt;
}

void ProjectRepository::update_project(int id, const std::string& name, const std::string& description, const std::string& start_date, const std::string& end_date) 
{
    SQLite::Database db("data/database.db", SQLite::OPEN_READWRITE);
    SQLite::Statement query(db, "UPDATE projects SET name = ?, description = ?, start_date = ?, end_date = ? WHERE id = ?");
    query.bind(1, name);
    query.bind(2, description);
    query.bind(3, start_date);
    query.bind(4, end_date);
    query.bind(5, id);
    query.exec();
}

void ProjectRepository::delete_project(int id) 
{
    SQLite::Database db("data/database.db", SQLite::OPEN_READWRITE);
    SQLite::Statement query(db, "DELETE FROM projects WHERE id = ?");
    query.bind(1, id);
    query.exec();
}

void ProjectRepository::delete_all_projects() 
{
    SQLite::Database db("data/database.db", SQLite::OPEN_READWRITE);
    db.exec("DELETE FROM projects");
}




// Manage project members
void ProjectRepository::add_user_to_project(int user_id, int project_id) 
{
    SQLite::Database db("data/database.db", SQLite::OPEN_READWRITE);
    SQLite::Statement query(db, "INSERT INTO work_on_project (user_id, project_id) VALUES (?, ?)");
    query.bind(1, user_id);
    query.bind(2, project_id);
    query.exec();
}

void ProjectRepository::remove_user_from_project(int user_id, int project_id) 
{
    SQLite::Database db("data/database.db", SQLite::OPEN_READWRITE);
    SQLite::Statement query(db, "DELETE FROM work_on_project WHERE user_id = ? AND project_id = ?");
    query.bind(1, user_id);
    query.bind(2, project_id);
    query.exec();
}

std::vector<User> ProjectRepository::get_project_users(int project_id) 
{
    std::vector<User> users;
    SQLite::Database db("data/database.db", SQLite::OPEN_READONLY);
    SQLite::Statement query(db, 
        "SELECT u.id, u.name, u.email, u.role, u.profile_picture "
        "FROM users u "
        "JOIN work_on_project w ON u.id = w.user_id "
        "WHERE w.project_id = ?");
    query.bind(1, project_id);
    
    while (query.executeStep()) 
    {
        User user;
        user.id              = query.getColumn(0);
        user.name            = query.getColumn(1).getText();
        user.email           = query.getColumn(2).getText();
        user.role            = static_cast<UserRole>(query.getColumn(3).getInt());
        user.profile_picture = query.getColumn(4).getText();
        users.push_back(user);
    }
    
    return users;
}

std::vector<Project> ProjectRepository::get_user_projects(int user_id) 
{
    std::vector<Project> projects;
    SQLite::Database db("data/database.db", SQLite::OPEN_READONLY);
    SQLite::Statement query(db, 
        "SELECT p.id, p.name, p.description, p.start_date, p.end_date "
        "FROM projects p "
        "JOIN work_on_project w ON p.id = w.project_id "
        "WHERE w.user_id = ?");
    query.bind(1, user_id);
    
    while (query.executeStep()) 
    {
        Project project;
        project.id          = query.getColumn(0);
        project.name        = query.getColumn(1).getText();
        project.description = query.getColumn(2).getText();
        project.start_date  = query.getColumn(3).getText();
        project.end_date    = query.getColumn(4).getText();
        projects.push_back(project);
    }
    
    return projects;
}