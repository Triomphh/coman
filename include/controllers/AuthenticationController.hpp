#pragma once

#include "../services/IAuthenticationService.hpp"
#include "../services/IUserService.hpp"
#include "../../third_party/crow_all.h"
#include <memory>


class AuthenticationController
{
private:
    std::shared_ptr<IAuthenticationService> authentication_service;
    std::shared_ptr<IUserService> user_service;
    crow::App<crow::CookieParser, crow::SessionMiddleware<crow::FileStore>>& app;

public:
    AuthenticationController(
        std::shared_ptr<IAuthenticationService> authentication_service,
        std::shared_ptr<IUserService> user_service,
        crow::App<crow::CookieParser, crow::SessionMiddleware<crow::FileStore>>& app
    );
    
    void register_routes(crow::App<crow::CookieParser, crow::SessionMiddleware<crow::FileStore>>& app);

private:
    crow::response handle_login(const crow::request& req);
    crow::response handle_login_page(const crow::request& req);
    crow::response handle_register(const crow::request& req);
    crow::response handle_register_page(const crow::request& req);
    crow::response handle_logout(const crow::request& req);
};