#pragma once

#include "../models/Task.hpp"
#include <string>
#include <optional>
#include <vector>


class ITaskService
{
public:
    virtual ~ITaskService() = default;

    virtual void create_task(const std::string& title, const std::string& description, TaskPriority priority, TaskStatus status, const std::string& deadline, int project_id) = 0;
    virtual std::optional<Task> get_task(int id) = 0;
    virtual std::vector<Task> get_all_tasks() = 0;
    virtual std::vector<Task> get_all_tasks_by_project(int project_id) = 0;
    virtual void update_task(int id, const std::string& title, const std::string& description, TaskPriority priority, TaskStatus status, const std::string& deadline, int project_id) = 0;
    virtual void delete_task(int id) = 0;
    virtual void delete_all_tasks() = 0;
};