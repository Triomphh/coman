#include "../../include/controllers/DashboardController.hpp"
#include "../../include/utils/ContextUtils.hpp"


DashboardController::DashboardController(
    std::shared_ptr<IProjectService> project_service,
    std::shared_ptr<ITaskService> task_service,
    std::shared_ptr<IUserService> user_service
) : project_service(std::move(project_service)),
    task_service(std::move(task_service)),
    user_service(std::move(user_service)) {}


void DashboardController::register_routes(
    crow::App<crow::CookieParser, crow::SessionMiddleware<crow::FileStore>>& app) 
{
    CROW_ROUTE(app, "/")
        ([this](const crow::request& req) {
            return handle_dashboard(req);
        });
}



crow::response DashboardController::handle_dashboard(const crow::request& req)
{
    try 
    {
        // Get session from request context
        auto& session_ctx = *reinterpret_cast<crow::detail::context<crow::CookieParser, crow::SessionMiddleware<crow::FileStore>>*>(req.middleware_context);
        auto& session = session_ctx.template get<crow::SessionMiddleware<crow::FileStore>>();
        
        // Check if user is logged in
        auto username = session.get("user", "");
        if (username.empty()) {
            // User is not logged in, redirect to login page
            crow::response redirect(302);
            redirect.set_header("Location", "/login");
            return redirect;
        }

        // User is logged in, proceed with dashboard rendering
        crow::mustache::context context;
        utils::ContextUtils::populate_navbar_context(context, req, user_service);
        
        // Get user data 
        auto user = user_service->get_user_by_email(username);
        if (user) {
            context["name"] = user->name;
        }

        auto page = crow::mustache::load("dashboard.html").render(context);
        return page;
    }
    catch (const std::exception& e)
    {
        return crow::response(500, "Internal server error: " + std::string(e.what()));
    }
}