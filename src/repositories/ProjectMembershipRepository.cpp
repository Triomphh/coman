#include "../include/repositories/ProjectMembershipRepository.hpp"


void ProjectMembershipRepository::add_user(int user_id, int project_id)
{
    SQLite::Statement query(*db, "INSERT INTO work_on_project (user_id, project_id) VALUES (?, ?)");
    query.bind(1, user_id);
    query.bind(2, project_id);
    query.exec();
}

void ProjectMembershipRepository::remove_user(int user_id, int project_id)
{
    SQLite::Statement query(*db, "DELETE FROM work_on_project WHERE user_id = ? AND project_id = ?");
    query.bind(1, user_id);
    query.bind(2, project_id);
    query.exec();
}

std::vector<User> ProjectMembershipRepository::get_project_users(int project_id)
{
    std::vector<User> users;
    SQLite::Statement query(*db, 
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

std::vector<Project> ProjectMembershipRepository::get_user_projects(int user_id)
{
    std::vector<Project> projects;
    SQLite::Statement query(*db, 
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