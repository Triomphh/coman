#pragma once

#include "../../third_party/crow_all.h"



class DocumentationController
{
public:
    DocumentationController() = default;
    void register_routes(crow::App<crow::CookieParser, crow::SessionMiddleware<crow::FileStore>>& app);

private:
    crow::response handle_documentation(const crow::request& req);
}; 