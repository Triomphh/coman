#pragma once

#include "../models/Task.hpp"
#include <optional>
#include <vector>


class ITaskRepository
{
public:
    virtual ~ITaskRepository() = default;

    // CRUD operations
    virtual void create(const Task& task, int project_id) = 0;
    virtual std::optional<Task> get(int id) = 0;
    virtual std::vector<Task> get_all() = 0;
    virtual std::optional<Task> get_by_project(int project_id, int task_id) = 0;
    virtual std::vector<Task> get_all_by_project(int project_id) = 0;
    virtual void update(const Task& task) = 0;
    virtual void remove(int id) = 0;
    virtual void remove_all() = 0;
};