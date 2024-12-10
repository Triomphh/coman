#pragma once

#include "../services/IUserService.hpp"
#include "../../third_party/crow_all.h"
#include <memory>


class UserController
{
private:
    std::shared_ptr<IUserService> user_service;

public:
    explicit UserController(std::shared_ptr<IUserService> user_service);
    void register_routes(crow::App<crow::CookieParser, crow::SessionMiddleware<crow::FileStore>>& app);

    crow::response handle_create_user(const crow::request& req);
    crow::response handle_get_all_users(const crow::request& req);
    crow::response handle_update_user(const crow::request& req, int id);
    crow::response handle_delete_user(const crow::request& req, int id);
    crow::response handle_delete_all_users(const crow::request& req);

    crow::response handle_get_profile(const crow::request& req);
};