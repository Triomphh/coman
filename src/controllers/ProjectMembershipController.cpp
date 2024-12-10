#include "../include/controllers/ProjectMembershipController.hpp"


ProjectMembershipController::ProjectMembershipController(std::shared_ptr<IProjectMembershipService> service) : service(std::move(service)) {}


void ProjectMembershipController::register_routes(crow::App<crow::CookieParser, crow::SessionMiddleware<crow::FileStore>>& app)
{
    CROW_ROUTE(app, "/projects/<int>/users/add")
        .methods("POST"_method)
        ([this](const crow::request& req, int project_id) 
        { 
            return handle_add_user_to_project(req, project_id); 
        });
    
    CROW_ROUTE(app, "/projects/<int>/users/<int>/remove")
        .methods("DELETE"_method)
        ([this](const crow::request& req, int project_id, int user_id) 
        { 
            return handle_remove_user_from_project(req, project_id, user_id); 
        });

    CROW_ROUTE(app, "/projects/<int>/users")
        ([this](const crow::request& req, int project_id) 
        { 
            return handle_get_project_users(req, project_id); 
        });

    CROW_ROUTE(app, "/users/<int>/projects")
        ([this](const crow::request& req, int user_id) 
        { 
            return handle_get_user_projects(req, user_id); 
        });
}


crow::response ProjectMembershipController::handle_add_user_to_project(const crow::request& req, int project_id)
{
    auto body = crow::json::load(req.body);
    if (!body) 
        return crow::response(400, "Invalid JSON body");
    
    int user_id = body["user_id"].i();
    try
    {
        service->add_user_to_project(user_id, project_id);
        return crow::response(200, "User added to project");
    }
    catch (const std::exception& e)
    {
        return crow::response(500, "Failed to add user to project");
    }
}


crow::response ProjectMembershipController::handle_remove_user_from_project(const crow::request& req, int project_id, int user_id)
{
    try
    {
        service->remove_user_from_project(user_id, project_id);
        return crow::response(200, "User removed from project");
    }
    catch (const std::exception& e)
    {
        return crow::response(500, "Failed to remove user from project");
    }
}


crow::response ProjectMembershipController::handle_get_project_users(const crow::request& req, int project_id)
{
    try
    {
        auto users = service->get_project_users(project_id);
        std::vector<crow::json::wvalue> user_list;

        for (const auto& user : users)
        {
            crow::json::wvalue u;
            u["id"]              = user.id;
            u["name"]            = user.name;
            u["email"]           = user.email;
            u["role"]            = static_cast<int>(user.role);
            u["profile_picture"] = user.profile_picture;
            user_list.push_back(std::move(u));
        }

        crow::json::wvalue response;
        response["users"] = std::move(user_list);
        return crow::response(200, response);
    }
    catch (const std::exception& e)
    {
        return crow::response(500, "Failed to get project users");
    }
}



crow::response ProjectMembershipController::handle_get_user_projects(const crow::request& req, int user_id)
{
    // TODO: implement
    return crow::response(500, "Not implemented");
}