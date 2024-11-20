#pragma once

#include "Task.hpp"
#include <vector>
#include <optional>

class TaskRepository
{
public:
    // CRUD operations
    static void create_task(const Task& task, int project_id);
    static std::vector<Task> get_tasks();
    static std::optional<Task> get_task(int id);
    static std::vector<Task> get_tasks_by_project(int project_id);
    static std::optional<Task> get_task_by_project(int project_id, int task_id);
    static void update_task(const Task& task);
    static void delete_task(int id);
    static void delete_all_tasks();
};
