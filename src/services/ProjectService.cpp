#include "../include/services/ProjectService.hpp"


ProjectService::ProjectService(std::shared_ptr<IProjectRepository> repo) : repository(std::move(repo)) {}


Project ProjectService::create_project(const std::string& name, const std::string& description, const std::string& start_date, const std::string& end_date)
{
    // TODO: validate input

    Project project;
    project.name        = name;
    project.description = description;
    project.start_date  = start_date;
    project.end_date    = end_date;

    repository->create(project);

    return project;
}


std::optional<Project> ProjectService::get_project(int id)
{
    // TODO: validate input

    return repository->get(id);
}


std::vector<Project> ProjectService::get_all_projects()
{
    return repository->get_all();
}


void ProjectService::update_project(int id, const std::string& name, const std::string& description, const std::string& start_date, const std::string& end_date)
{
    // TODO: validate input

    Project project;
    project.id          = id;
    project.name        = name;
    project.description = description;
    project.start_date  = start_date;
    project.end_date    = end_date;

    repository->update(project);
}


void ProjectService::delete_project(int id)
{
    // TODO: validate input

    repository->remove(id);
}


void ProjectService::delete_all_projects()
{
    repository->remove_all();
}