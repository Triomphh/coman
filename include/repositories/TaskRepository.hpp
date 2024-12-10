#pragma once

#include "ITaskRepository.hpp"
#include <SQLiteCpp/SQLiteCpp.h>


class TaskRepository : public ITaskRepository
{
private:
    std::shared_ptr<SQLite::Database> db;

public:
    TaskRepository(std::shared_ptr<SQLite::Database> database) : db(std::move(database)) {}

    void create(const Task& task, int project_id) override;
    std::optional<Task> get(int id) override;
    std::vector<Task> get_all() override;
    std::optional<Task> get_by_project(int project_id, int task_id) override;
    std::vector<Task> get_all_by_project(int project_id) override;
    void update(const Task& task) override;
    void remove(int id) override;
    void remove_all() override;
};