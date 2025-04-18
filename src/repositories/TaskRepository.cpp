#include "../../include/repositories/TaskRepository.hpp"


void TaskRepository::create(const Task& task, int project_id)
{
    SQLite::Statement query(*db, "INSERT INTO tasks (title, description, priority, status, deadline, project_id) VALUES (?, ?, ?, ?, ?, ?)");
    query.bind(1, task.title);
    query.bind(2, task.description);
    query.bind(3, static_cast<int>(task.priority));
    query.bind(4, static_cast<int>(task.status));
    query.bind(5, task.deadline);
    query.bind(6, project_id);
    query.exec();
}

std::optional<Task> TaskRepository::get(int id)
{
    SQLite::Statement query(*db, "SELECT id, title, description, priority, status, deadline, project_id FROM tasks WHERE id = ?");
    query.bind(1, id);

    if (query.executeStep()) 
    {
        Task task;
        task.id          = query.getColumn(0);
        task.title       = query.getColumn(1).getText();
        task.description = query.getColumn(2).getText();
        task.priority    = static_cast<TaskPriority>(query.getColumn(3).getInt());
        task.status      = static_cast<TaskStatus>(query.getColumn(4).getInt());
        task.deadline    = query.getColumn(5).getText();

        return task;
    }

    return std::nullopt;
}

std::vector<Task> TaskRepository::get_all()
{
    std::vector<Task> tasks;
    SQLite::Statement query(*db, "SELECT id, title, description, priority, status, deadline, project_id FROM tasks");

    while (query.executeStep()) 
    {
        Task task;
        task.id          = query.getColumn(0);
        task.title       = query.getColumn(1).getText();
        task.description = query.getColumn(2).getText();
        task.priority    = static_cast<TaskPriority>(query.getColumn(3).getInt());
        task.status      = static_cast<TaskStatus>(query.getColumn(4).getInt());
        task.deadline    = query.getColumn(5).getText();
        tasks.push_back(task);
    }

    return tasks;
}

std::optional<Task> TaskRepository::get_by_project(int project_id, int task_id)
{
    SQLite::Statement query(*db, "SELECT id, title, description, priority, status, deadline, project_id FROM tasks WHERE project_id = ? AND id = ?");
    query.bind(1, project_id);
    query.bind(2, task_id);

    if (query.executeStep()) 
    {
        Task task;
        task.id          = query.getColumn(0);
        task.title       = query.getColumn(1).getText();
        task.description = query.getColumn(2).getText();
        task.priority    = static_cast<TaskPriority>(query.getColumn(3).getInt());
        task.status      = static_cast<TaskStatus>(query.getColumn(4).getInt());
        task.deadline    = query.getColumn(5).getText();
        return task;
    }

    return std::nullopt;
}

std::vector<Task> TaskRepository::get_all_by_project(int project_id)
{
    std::vector<Task> tasks;
    SQLite::Statement query(*db, "SELECT id, title, description, priority, status, deadline, project_id FROM tasks WHERE project_id = ?");
    query.bind(1, project_id);

    while (query.executeStep()) 
    {
        Task task;
        task.id          = query.getColumn(0);
        task.title       = query.getColumn(1).getText();
        task.description = query.getColumn(2).getText();
        task.priority    = static_cast<TaskPriority>(query.getColumn(3).getInt());
        task.status      = static_cast<TaskStatus>(query.getColumn(4).getInt());
        task.deadline    = query.getColumn(5).getText();
        tasks.push_back(task);
    }

    return tasks;
}

void TaskRepository::update(const Task& task)
{
    SQLite::Statement query(*db, "UPDATE tasks SET title = ?, description = ?, priority = ?, status = ?, deadline = ? WHERE id = ?");
    query.bind(1, task.title);
    query.bind(2, task.description);
    query.bind(3, static_cast<int>(task.priority));
    query.bind(4, static_cast<int>(task.status));
    query.bind(5, task.deadline);
    query.bind(6, task.id);
    query.exec();
}

void TaskRepository::remove(int id)
{
    SQLite::Statement query(*db, "DELETE FROM tasks WHERE id = ?");
    query.bind(1, id);
    query.exec();
}

void TaskRepository::remove_all()
{
    SQLite::Statement query(*db, "DELETE FROM tasks");
    query.exec();
}