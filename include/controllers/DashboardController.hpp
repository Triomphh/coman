#pragma once

#include "../services/IProjectService.hpp"
#include "../services/ITaskService.hpp"
#include "../services/IUserService.hpp"
#include "../../third_party/crow_all.h"

class DashboardController 
{
private:
    std::shared_ptr<IProjectService> project_service;
    std::shared_ptr<ITaskService> task_service;
    std::shared_ptr<IUserService> user_service;

public:
    DashboardController(std::shared_ptr<IProjectService> project_service,
                        std::shared_ptr<ITaskService> task_service,
                        std::shared_ptr<IUserService> user_service);
    void register_routes(crow::App<crow::CookieParser, crow::SessionMiddleware<crow::FileStore>>& app);

private:
    crow::response handle_dashboard(const crow::request& req);
    void populate_navbar_context(crow::mustache::context& ctx, 
                                 const crow::request& req, 
                                 crow::App<crow::CookieParser, crow::SessionMiddleware<crow::FileStore>>& app);
};