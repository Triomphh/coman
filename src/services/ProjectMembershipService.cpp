#include "../include/services/ProjectMembershipService.hpp"


ProjectMembershipService::ProjectMembershipService(std::shared_ptr<IProjectMembershipRepository> repo) : repository(std::move(repo)) {}


void ProjectMembershipService::add_user_to_project(int user_id, int project_id)
{
    // TODO: validate input

    repository->add_user(user_id, project_id);
}


void ProjectMembershipService::remove_user_from_project(int user_id, int project_id)
{
    // TODO: validate input

    repository->remove_user(user_id, project_id);
}


std::vector<User> ProjectMembershipService::get_project_users(int project_id)
{
    // TODO: validate input

    return repository->get_project_users(project_id);
}


std::vector<Project> ProjectMembershipService::get_user_projects(int user_id)
{
    // TODO: validate input

    return repository->get_user_projects(user_id);
}