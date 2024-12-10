#include "../include/repositories/ProjectRepository.hpp"


void ProjectRepository::create(const Project& project)
{
    SQLite::Statement query(*db, "INSERT INTO projects (name, description, start_date, end_date) VALUES (?, ?, ?, ?)");
    query.bind(1, project.name);
    query.bind(2, project.description);
    query.bind(3, project.start_date);
    query.bind(4, project.end_date);
    query.exec();
}

std::optional<Project> ProjectRepository::get(int id)
{
    SQLite::Statement query(*db, "SELECT id, name, description, start_date, end_date FROM projects WHERE id = ?");
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

std::vector<Project> ProjectRepository::get_all()
{
    std::vector<Project> projects;
    SQLite::Statement query(*db, "SELECT id, name, description, start_date, end_date FROM projects");

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

void ProjectRepository::update(const Project& project)
{
    SQLite::Statement query(*db, "UPDATE projects SET name = ?, description = ?, start_date = ?, end_date = ? WHERE id = ?");
    query.bind(1, project.name);
    query.bind(2, project.description);
    query.bind(3, project.start_date);
    query.bind(4, project.end_date);
    query.bind(5, project.id);
    query.exec();
}

void ProjectRepository::remove(int id)
{
    SQLite::Statement query(*db, "DELETE FROM projects WHERE id = ?");
    query.bind(1, id);
    query.exec();
}

void ProjectRepository::remove_all()
{
    SQLite::Statement query(*db, "DELETE FROM projects");
    query.exec();
}