#include "../../include/controllers/DocumentationController.hpp"



void DocumentationController::register_routes(crow::App<crow::CookieParser, crow::SessionMiddleware<crow::FileStore>>& app)
{
    CROW_ROUTE(app, "/doc")
        .methods("GET"_method)
        ([this](const crow::request& req) 
        {
            return handle_documentation(req);
        });
}


crow::response DocumentationController::handle_documentation(const crow::request& req)
{
    auto page = crow::mustache::load("presentation.html");
    return page.render();
} 