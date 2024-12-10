#pragma once

#include "../services/ITaskService.hpp"
#include "../../third_party/crow_all.h"
#include <memory>


class TaskController
{
private:
    std::shared_ptr<ITaskService> task_service;

public:
    explicit TaskController(std::shared_ptr<ITaskService> task_service);
    void register_routes(crow::App<crow::CookieParser, crow::SessionMiddleware<crow::FileStore>>& app);
    crow::response handle_get_tasks(const crow::request& req, int project_id);

private:
    crow::response handle_create_task(const crow::request& req, int project_id);
    crow::response handle_update_task(const crow::request& req, int project_id, int task_id);
    crow::response handle_delete_task(const crow::request& req, int project_id, int task_id);
    crow::response handle_delete_all_tasks(const crow::request& req);
};