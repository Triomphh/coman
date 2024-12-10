#include "../include/services/TaskService.hpp"


TaskService::TaskService(std::shared_ptr<ITaskRepository> repo) : repository(std::move(repo)) {}


void TaskService::create_task(const std::string& title, const std::string& description, TaskPriority priority, TaskStatus status, const std::string& deadline, int project_id)
{
    // TODO: validate input

    // Create task object
    Task task;
    task.title       = title;
    task.description = description;
    task.priority    = priority;
    task.status      = status;
    task.deadline    = deadline;

    repository->create(task, project_id);
}


std::optional<Task> TaskService::get_task(int id)
{
    // TODO: validate input

    return repository->get(id);
}


std::vector<Task> TaskService::get_all_tasks()
{
    return repository->get_all();
}


std::vector<Task> TaskService::get_all_tasks_by_project(int project_id)
{
    // TODO: validate input

    return repository->get_all_by_project(project_id);
}


void TaskService::update_task(int id, const std::string& title, const std::string& description, TaskPriority priority, TaskStatus status, const std::string& deadline, int project_id)
{
    // TODO: validate input

    Task task;
    task.id          = id;
    task.title       = title;
    task.description = description;
    task.priority    = priority;
    task.status      = status;
    task.deadline    = deadline;

    repository->update(task);
}


void TaskService::delete_task(int id)
{
    // TODO: validate input

    repository->remove(id);
}


void TaskService::delete_all_tasks()
{
    repository->remove_all();
}