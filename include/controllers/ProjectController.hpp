#pragma once

#include "../services/IProjectService.hpp"
#include "../services/IProjectMembershipService.hpp"
#include "../services/IUserService.hpp"
#include "TaskController.hpp"
#include "../../third_party/crow_all.h"
#include <memory>


class ProjectController
{
private:
    std::shared_ptr<IProjectService> project_service;
    std::shared_ptr<IProjectMembershipService> project_membership_service;
    std::shared_ptr<TaskController> task_controller;
    std::shared_ptr<IUserService> user_service;
public:
    explicit ProjectController(
        std::shared_ptr<IProjectService> project_service, 
        std::shared_ptr<IProjectMembershipService> project_membership_service, 
        std::shared_ptr<TaskController> task_controller,
        std::shared_ptr<IUserService> user_service
    );
    void register_routes(crow::App<crow::CookieParser, crow::SessionMiddleware<crow::FileStore>>& app);

private:
    crow::response handle_create_project(const crow::request& req);
    crow::response handle_get_project(const crow::request& req, int id);
    crow::response handle_get_all_projects(const crow::request& req);
    crow::response handle_update_project(const crow::request& req, int id);
    crow::response handle_delete_project(const crow::request& req, int id);
    crow::response handle_delete_all_projects(const crow::request& req);
};