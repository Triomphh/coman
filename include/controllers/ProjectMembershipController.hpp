#pragma once

#include "../services/IProjectMembershipService.hpp"
#include "../../third_party/crow_all.h"
#include <memory>

class ProjectMembershipController
{
private:
    std::shared_ptr<IProjectMembershipService> service;

public:
    explicit ProjectMembershipController(std::shared_ptr<IProjectMembershipService> service);
    void register_routes(crow::App<crow::CookieParser, crow::SessionMiddleware<crow::FileStore>>& app);

private:
    crow::response handle_add_user_to_project(const crow::request& req, int project_id);
    crow::response handle_remove_user_from_project(const crow::request& req, int project_id, int user_id);
    crow::response handle_get_project_users(const crow::request& req, int project_id);
    crow::response handle_get_user_projects(const crow::request& req, int user_id);
};