#include "../../include/controllers/AuthenticationController.hpp"


AuthenticationController::AuthenticationController(
    std::shared_ptr<IAuthenticationService> authentication_service, 
    std::shared_ptr<IUserService> user_service,
    crow::App<crow::CookieParser, crow::SessionMiddleware<crow::FileStore>>& app
) : authentication_service(std::move(authentication_service)), 
    user_service(std::move(user_service)),
    app(app) {}


void AuthenticationController::register_routes(crow::App<crow::CookieParser, crow::SessionMiddleware<crow::FileStore>>& app)
{
    CROW_ROUTE(app, "/login")
        .methods("GET"_method)
        ([this](const crow::request& req) 
        { 
            return handle_login_page(req); 
        });

    CROW_ROUTE(app, "/login")
        .methods("POST"_method)
        ([this](const crow::request& req) 
        { 
            return handle_login(req); 
        });

    CROW_ROUTE(app, "/register")
        .methods("GET"_method)
        ([this](const crow::request& req) 
        { 
            return handle_register_page(req); 
        });

    CROW_ROUTE(app, "/register")
        .methods("POST"_method)
        ([this](const crow::request& req) 
        { 
            return handle_register(req); 
        });

    CROW_ROUTE(app, "/logout")
        .methods("POST"_method)
        ([this](const crow::request& req) 
        { 
            return handle_logout(req); 
        });
}



crow::response AuthenticationController::handle_login_page(const crow::request& req)
{
    auto& session = app.get_context<crow::SessionMiddleware<crow::FileStore>>(req);
    
    // If user is already logged in, redirect to home page
    if (!session.get("user", "").empty()) {
        crow::response response(302);
        response.set_header("Location", "/");
        return response;
    }

    // If user is not logged in, render login page
    return crow::mustache::load("login.html").render();
}


crow::response AuthenticationController::handle_login(const crow::request& req)
{
    auto& session = app.get_context<crow::SessionMiddleware<crow::FileStore>>(req);

    auto params = req.get_body_params();
    std::string login = params.get("login");
    std::string password = params.get("password");

    crow::response response(302);

    // Validate credentials
    auto user = authentication_service->authenticate(login, password);
    if (user)
    {
        session.set("user", user->email);
        response.set_header("Location", "/");
    }
    else
    {
        response.set_header("Location", "/login");
    }
    return response;
}


crow::response AuthenticationController::handle_register_page(const crow::request& req)
{
    auto& session = app.get_context<crow::SessionMiddleware<crow::FileStore>>(req);
    
    // If user is already logged in, redirect to home page
    if (!session.get("user", "").empty()) {
        crow::response response(303);
        response.set_header("Location", "/");
        return response;
    }

    // If user is not logged in, render registration page
    crow::mustache::context context;
    // Add error message if present in the session
    if (auto error = session.get("register_error", ""); !error.empty()) {
        context["error"] = error;
        session.remove("register_error");
    }
    return crow::mustache::load("register.html").render(context);
}


crow::response AuthenticationController::handle_register(const crow::request& req)
{
    auto& session = app.get_context<crow::SessionMiddleware<crow::FileStore>>(req);
    auto params = req.get_body_params();
    
    std::string name = params.get("name");
    std::string email = params.get("email");
    std::string password = params.get("password");

    crow::response response;
    response.code = 303; // Redirect after POST

    // Basic validation
    if (name.empty() || email.empty() || password.empty()) {
        session.set("register_error", "All fields are required");
        response.set_header("Location", "/register");
        return response;
    }

    try {
        // Check if user already exists
        if (user_service->get_user_by_email(email)) {
            session.set("register_error", "Email already registered");
            response.set_header("Location", "/register");
            return response;
        }

        // Create new user (default role as TEAM_MEMBER)
        user_service->create_user(name, email, password, UserRole::TEAM_MEMBER, "default.png");
        
        // Log the user in automatically
        session.set("user", email);
        response.set_header("Location", "/");
    }
    catch (const std::exception& e) {
        session.set("register_error", "Registration failed. Please try again.");
        response.set_header("Location", "/register");
    }

    return response;
}


crow::response AuthenticationController::handle_logout(const crow::request& req)
{
    auto& session = app.get_context<crow::SessionMiddleware<crow::FileStore>>(req);
    session.remove("user");
    
    crow::response response(302);
    response.set_header("Location", "/login");
    return response;
}



