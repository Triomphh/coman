#include "../../include/controllers/UserController.hpp"
#include "../../include/utils/ContextUtils.hpp"


UserController::UserController(std::shared_ptr<IUserService> user_service) : user_service(std::move(user_service)) {}


void UserController::register_routes(crow::App<crow::CookieParser, crow::SessionMiddleware<crow::FileStore>>& app)
{
    CROW_ROUTE(app, "/users/create")
        .methods("POST"_method)
        ([this](const crow::request& req)
        {
            return handle_create_user(req);
        });
    
    CROW_ROUTE(app, "/users")
        ([this](const crow::request& req) 
        { 
            return handle_get_all_users(req); 
        });

    CROW_ROUTE(app, "/users/json")
        ([this](const crow::request& req) 
        { 
            return handle_get_all_users_json(req); 
        });

    CROW_ROUTE(app, "/users/delete/<int>")
        .methods("DELETE"_method)
        ([this](const crow::request& req, int id) 
        { 
            return handle_delete_user(req, id); 
        });

    CROW_ROUTE(app, "/users/deleteall")
        .methods("DELETE"_method)
        ([this](const crow::request& req) 
        { 
            return handle_delete_all_users(req); 
        });

    CROW_ROUTE(app, "/profile")
        .methods("GET"_method)
        ([this](const crow::request& req)
        {
            return handle_get_profile(req);
        });
}



crow::response UserController::handle_create_user(const crow::request& req)
{
    try
    {
        auto body = crow::json::load(req.body);
        if (!body)
        {
            return crow::response(400, "Invalid JSON body");
        }

        auto user = user_service->create_user(
            body["name"].s(),
            body["email"].s(),
            body["password"].s(),
            static_cast<UserRole>(body["role"].i()),
            body.has("profile_picture") ? std::string(body["profile_picture"].s()) : std::string("default.png")
        );

        return crow::response(201, "User created");
    }
    catch (const std::exception& e)
    {
        return crow::response(500, "Internal server error: " + std::string(e.what()));
    }
}


crow::response UserController::handle_get_all_users(const crow::request& req)
{
    try
    {
        crow::mustache::context context;
        utils::ContextUtils::populate_navbar_context(context, req, user_service);

        auto users = user_service->get_all_users();
        std::vector<crow::json::wvalue> user_list;

        for (const auto& user : users)
        {
            crow::json::wvalue u;
            u["id"]    = user.id;
            u["name"]  = user.name;
            u["email"]           = user.email;
            u["profile_picture"] = user.profile_picture;
            u["role"]            = [&user]() {
                switch (user.role) {
                    case UserRole::ADMIN:           return "Admin";
                    case UserRole::PROJECT_MANAGER: return "Project Manager";
                    case UserRole::TEAM_MEMBER:     return "Team Member";
                    default:                        return "Unknown";
                }
            }();

            // Add role-specific flags for styling
            u["isAdmin"]          = (user.role == UserRole::ADMIN);
            u["isProjectManager"] = (user.role == UserRole::PROJECT_MANAGER);
            u["isTeamMember"]     = (user.role == UserRole::TEAM_MEMBER);

            user_list.push_back(std::move(u));
        }

        context["users"] = std::move(user_list);
        auto page = crow::mustache::load("users.html").render(context);
        return page;
    }
    catch (const std::exception& e)
    {
        return crow::response(500, "Internal server error: " + std::string(e.what()));
    }
}


crow::response UserController::handle_get_all_users_json(const crow::request& req)
{
    try
    {
        auto users = user_service->get_all_users();
        std::vector<crow::json::wvalue> user_list;

        for (const auto& user : users)
        {
            crow::json::wvalue u;
            u["id"] = user.id;
            u["name"] = user.name;
            u["email"] = user.email;
            u["profile_picture"] = user.profile_picture;
            u["role"] = static_cast<int>(user.role);
            user_list.push_back(std::move(u));
        }

        crow::json::wvalue response;
        response["users"] = std::move(user_list);
        return crow::response(200, response);
    }
    catch (const std::exception& e)
    {
        return crow::response(500, "Internal server error: " + std::string(e.what()));
    }
}


crow::response UserController::handle_delete_user(const crow::request& req, int id)
{
    try
    {
        user_service->delete_user(id);
        return crow::response(200, "User deleted");
    }
    catch (const std::exception& e)
    {
        return crow::response(500, "Internal server error: " + std::string(e.what()));
    }
}


crow::response UserController::handle_delete_all_users(const crow::request& req)
{
    try
    {
        user_service->delete_all_users();
        return crow::response(200, "All users deleted");
    }
    catch (const std::exception& e)
    {
        return crow::response(500, "Internal server error: " + std::string(e.what()));
    }
}



crow::response UserController::handle_get_profile(const crow::request& req)
{
    auto& session = *reinterpret_cast<crow::detail::context<crow::CookieParser, crow::SessionMiddleware<crow::FileStore>>*>(req.middleware_context);
    auto username = session.template get<crow::SessionMiddleware<crow::FileStore>>().get("user", "");
    
    // If user is not logged in, redirect to login page
    if (username.empty()) 
    {
        crow::response redirect(302);
        redirect.set_header("Location", "/login");
        return redirect;
    }
    
    // Get user data
    auto user = user_service->get_user_by_email(username);
    if (!user) 
    {
        crow::response redirect(302);
        redirect.set_header("Location", "/login");
        return redirect;
    }
    
    crow::mustache::context context;
    utils::ContextUtils::populate_navbar_context(context, req, user_service);
    
    // Add user data to context
    context["name"]            = user->name;
    context["email"]           = user->email;
    context["profile_picture"] = user->profile_picture;
    
    // Add role-specific flags for styling
    context["isAdmin"]          = (user->role == UserRole::ADMIN);
    context["isProjectManager"] = (user->role == UserRole::PROJECT_MANAGER);
    context["isTeamMember"]     = (user->role == UserRole::TEAM_MEMBER);
    
    // Add role text
    switch(user->role) 
    {
        case UserRole::ADMIN:           context["roleText"] = "Admin"; break;
        case UserRole::PROJECT_MANAGER: context["roleText"] = "Project Manager"; break;
        case UserRole::TEAM_MEMBER:     context["roleText"] = "Team Member"; break;
    }
    
    return crow::mustache::load("profile.html").render(context);
}