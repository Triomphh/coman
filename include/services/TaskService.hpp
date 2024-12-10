#pragma once

#include "ITaskService.hpp"
#include "../repositories/ITaskRepository.hpp"
#include <memory>


class TaskService : public ITaskService
{
private:
    std::shared_ptr<ITaskRepository> repository;

public:
    explicit TaskService(std::shared_ptr<ITaskRepository> repo);
    ~TaskService() override = default;

    void create_task(const std::string& title, const std::string& description, TaskPriority priority, TaskStatus status, const std::string& deadline, int project_id) override;
    std::optional<Task> get_task(int id) override;
    std::vector<Task> get_all_tasks() override;
    std::vector<Task> get_all_tasks_by_project(int project_id) override;
    void update_task(int id, const std::string& title, const std::string& description, TaskPriority priority, TaskStatus status, const std::string& deadline, int project_id) override;
    void delete_task(int id) override;
    void delete_all_tasks() override;
};