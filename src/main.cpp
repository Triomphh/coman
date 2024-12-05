#include "../third_party/crow_all.h"    // Using Crow framework to manage HTTP web services (taking care of routing, multithreading, etc...)
#include "../include/ProjectRepository.hpp"
#include "../include/TaskRepository.hpp"
#include "../include/UserRepository.hpp"
#include "../include/DatabaseManager.hpp"



// Used to populate the navbar context with user information
void populate_navbar_context(crow::mustache::context& ctx, const crow::request& req, crow::App<crow::CookieParser, crow::SessionMiddleware<crow::FileStore>>& app) 
{
    auto& session = app.get_context<crow::SessionMiddleware<crow::FileStore>>(req);
    auto user = UserRepository::get_user_by_email(session.get("user", ""));
    if (user) 
    {
        ctx["profile_picture"] = user->profile_picture;
        ctx["name"] = user->name;
    }
}



int main()
{
    // Define the session middleware
    using Session = crow::SessionMiddleware<crow::FileStore>;

    // Define the Crow application
    crow::App<crow::CookieParser, Session> app { Session{crow::FileStore("./sessions")} };


    // app.loglevel(crow::LogLevel::Info);    // Show all logs bellow this level (see https://crowcpp.org/master/guides/logging/ for more info)


    // Initialize database - Singleton pattern (table creation done in the constructor of DatabaseManager)
    DatabaseManager::getInstance();


    // Define differents endpoints

    // Dashboard   if user is logged in
    // Login page  if user is not logged in
    CROW_ROUTE(app, "/")([&](const crow::request& req) -> crow::response
    {
        auto& session = app.get_context<Session>(req);
        auto username = session.get("user", "");

        // If user is not logged in, redirect to login page
        if (username.empty()) {
            crow::response redirect(302);
            redirect.set_header("Location", "/login");
            return redirect;
        }

        // Else, display the dashboard
        crow::mustache::context context;
        populate_navbar_context(context, req, app);

        // -- Recent projects --------------------------------------------------- 

        // ----------------------------------------------------------------------


        // -- Recent tasks ------------------------------------------------------

        // ----------------------------------------------------------------------

        auto page = crow::mustache::load("dashboard.html").render(context);
        return page;
    });


    CROW_ROUTE(app, "/login").methods("GET"_method)([&](const crow::request& req) -> crow::response
    {
        auto& session = app.get_context<Session>(req);
        
        // If user is already logged in, redirect to home page
        if (!session.get("user", "").empty()) {
            crow::response response(302);
            response.set_header("Location", "/");
            return response;
        }

        // If user is not logged in, render login page
        return crow::mustache::load("login.html").render();
    });


    CROW_ROUTE(app, "/login").methods("POST"_method)([&](const crow::request& req)
    {
        auto& session = app.get_context<Session>(req);

        auto params = req.get_body_params();
        std::string login    = params.get("login");
        std::string password = params.get("password");

        crow::response response(302);

        // Validate credentials
        auto user = UserRepository::authenticate(login, password);
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
    });


    CROW_ROUTE(app, "/logout").methods("POST"_method)([&](const crow::request& req) 
    {
        auto& session = app.get_context<Session>(req);
        session.remove("user");
        
        crow::response response(302);
        response.set_header("Location", "/login");
        return response;
    });


    CROW_ROUTE(app, "/register").methods("GET"_method)([&](const crow::request& req) -> crow::response
    {
        auto& session = app.get_context<Session>(req);
        
        // If user is already logged in, redirect to home page
        if (session.get("user", "") != "")
        {
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
    });


    CROW_ROUTE(app, "/register").methods("POST"_method)([&](const crow::request& req)
    {
        auto& session = app.get_context<Session>(req);
        auto params = req.get_body_params();
        std::string name     = params.get("name");
        std::string email    = params.get("email");
        std::string password = params.get("password");

        crow::response response;
        response.code = 303;    // Redirect after POST

        // Basic validation
        if (name.empty() || email.empty() || password.empty()) 
        {
            session.set("register_error", "All fields are required");
            response.set_header("Location", "/register");
            return response;
        }

        try 
        {
            // Check if user already exists
            if (UserRepository::get_user_by_email(email)) 
            {
                session.set("register_error", "Email already registered");
                response.set_header("Location", "/register");
                return response;
            }

            // Create new user (default role as TEAM_MEMBER)
            UserRepository::create_user(name, email, password, UserRole::TEAM_MEMBER);
            
            // Log the user in automatically
            session.set("user", email);
            response.set_header("Location", "/");
        }
        catch (const std::exception& e) 
        {
            session.set("register_error", "Registration failed. Please try again.");
            response.set_header("Location", "/register");
        }

        return response;
    });





    CROW_ROUTE(app, "/projects")([&](const crow::request& req) 
    {
        crow::mustache::context context;
        populate_navbar_context(context, req, app);
        
        auto projects = ProjectRepository::get_projects();
        std::vector<crow::json::wvalue> project_list;
        
        for (const auto& project : projects) {
            crow::json::wvalue p;
            p["id"]            = project.id;
            p["name"]          = project.name;
            p["description"]   = project.description;
            p["start_date"]    = project.start_date;
            p["end_date"]      = project.end_date;

            // Get users for this project
            auto project_users = ProjectRepository::get_project_users(project.id);
            std::vector<crow::json::wvalue> user_avatars;
            
            for (const auto& user : project_users) {
                crow::json::wvalue avatar;
                avatar["profile_picture"] = "/static/profile_pictures/" + user.profile_picture;
                avatar["name"] = user.name; 
                user_avatars.push_back(std::move(avatar));
            }
            
            p["users"] = std::move(user_avatars);
            project_list.push_back(std::move(p));
        }
        
        context["projects"] = std::move(project_list);
        auto page = crow::mustache::load("projects.html").render(context);
        return page;
    });


    CROW_ROUTE(app, "/projects/<int>")([&](const crow::request& req, int project_id) 
    {
        crow::mustache::context context;
        populate_navbar_context(context, req, app);
        
        auto project = ProjectRepository::get_project(project_id);

        // Add project details
        context["id"]            = project->id;
        context["name"]          = project->name;
        context["description"]   = project->description;
        context["start_date"]    = project->start_date;
        context["end_date"]      = project->end_date;

        // Get tasks for this project
        auto tasks = TaskRepository::get_tasks_by_project(project_id);

        std::vector<crow::json::wvalue> task_list;
        for (const auto& task : tasks) 
        {
            crow::json::wvalue t;
            t["id"]            = task.id;
            t["title"]         = task.title;
            t["description"]   = task.description;
            t["deadline"]      = task.deadline;
            t["status"] = static_cast<int>(task.status);
            
            // Add status helper booleans
            t["isStatusTodo"]       = (task.status == TaskStatus::TODO);
            t["isStatusInProgress"] = (task.status == TaskStatus::IN_PROGRESS);
            t["isStatusDone"]       = (task.status == TaskStatus::DONE);

            // Add priority as a string and individual flags
            t["priorityHigh"]   = (task.priority == TaskPriority::HIGH);
            t["priorityMedium"] = (task.priority == TaskPriority::MEDIUM);
            t["priorityLow"]    = (task.priority == TaskPriority::LOW);
            
            // Convert priority to display string
            switch(task.priority) {
                case TaskPriority::HIGH:   t["priorityText"] = "High"; break;
                case TaskPriority::MEDIUM: t["priorityText"] = "Medium"; break;
                case TaskPriority::LOW:    t["priorityText"] = "Low"; break;
            }
            
            task_list.push_back(std::move(t));
        }
        
        context["tasks"] = std::move(task_list);

        // Get project users
        auto project_users = ProjectRepository::get_project_users(project_id);
        std::vector<crow::json::wvalue> user_list;
        
        for (const auto& user : project_users) {
            crow::json::wvalue u;
            u["name"] = user.name;
            u["profile_picture"] = user.profile_picture;
            user_list.push_back(std::move(u));
        }
        
        context["users"] = std::move(user_list);

        auto page = crow::mustache::load("project_details.html").render(context);
        return page;
    });


    CROW_ROUTE(app, "/projects/<int>/tasks/create").methods("POST"_method)([](const crow::request& req, int project_id) 
    {
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, "Invalid JSON body");
        }

        Task task;
        task.title       = body["title"].s();
        task.description = body["description"].s();
        task.priority    = static_cast<TaskPriority>(body["priority"].i());
        task.status      = TaskStatus::TODO;  // New tasks start as TODO
        task.deadline    = body["deadline"].s();

        TaskRepository::create_task(task, project_id);

        return crow::response(201, "Task created");
    });
    // curl -X POST http://localhost:18080/projects/1/tasks/create -d '{"title": "Test", "description": "Test description", "priority": 1, "deadline": "2024-12-31"}'


    CROW_ROUTE(app, "/projects/<int>/tasks")([](int project_id)
    {
        auto tasks = TaskRepository::get_tasks_by_project(project_id);
        crow::mustache::context context;
        std::vector<crow::json::wvalue> task_list;

        for (const auto& task : tasks) {
            crow::json::wvalue t;
            t["id"]            = task.id;
            t["title"]         = task.title;
            t["description"]   = task.description;
            t["deadline"]      = task.deadline;
            t["status"]        = static_cast<int>(task.status);
            task_list.push_back(std::move(t));
        }

        context["tasks"] = std::move(task_list);
        return context;
    });
    // curl -X GET http://localhost:18080/projects/1/tasks


    CROW_ROUTE(app, "/projects/<int>/tasks/<int>/update").methods("PUT"_method)([](const crow::request& req, int project_id, int task_id) 
    {
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, "Invalid JSON body");
        }

        try {
            Task task;
            task.id          = task_id;
            task.title       = body["title"].s();
            task.description = body["description"].s();
            task.priority    = static_cast<TaskPriority>(body["priority"].i());
            task.deadline    = body["deadline"].s();
            task.status      = static_cast<TaskStatus>(body["status"].i());
            
            TaskRepository::update_task(task);
            return crow::response(200, "Task updated successfully");
        } 
        catch (const std::exception& e) {
            return crow::response(500, "Internal server error");
        }
    });
    // curl -X PUT http://localhost:18080/projects/1/tasks/1/update -d '{"title": "Test", "description": "Test description", "priority": 1, "deadline": "2024-12-31", "status": 1}'


    CROW_ROUTE(app, "/projects/<int>/tasks/<int>/delete").methods("DELETE"_method)([](const crow::request& req, int project_id, int task_id) 
    {
        try 
        {
            TaskRepository::delete_task(task_id);
            return crow::response(204);
        } 
        catch (const std::exception& e) 
        {
            return crow::response(500, "Internal server error");
        }
    });
    // curl -X DELETE http://localhost:18080/projects/1/tasks/1/delete


    CROW_ROUTE(app, "/projects/create").methods("POST"_method)([](const crow::request& req)
    {
        auto body = crow::json::load(req.body);
        if (!body)
        {
            return crow::response(crow::status::BAD_REQUEST);
        }

        std::string name        = body["name"].s();
        std::string description = body["description"].s();
        std::string start_date  = body["start_date"].s();
        std::string end_date    = body["end_date"].s();

        ProjectRepository::create_project(name, description, start_date, end_date);

        return crow::response(201, "Project created");
    });
    // curl -X POST http://localhost:18080/projects/create -d '{"name": "Test", "description": "Test description", "start_date": "2024-03-20", "end_date": "2024-12-31"}'


    // Add this new endpoint for updating projects
    CROW_ROUTE(app, "/projects/<int>/update").methods("PUT"_method)([](const crow::request& req, int id) 
    {
        auto body = crow::json::load(req.body);
        if (!body)
            return crow::response(400, "Invalid JSON body");

        try 
        {
            std::string name = body["name"].s();
            std::string description = body["description"].s();
            std::string start_date = body["start_date"].s();
            std::string end_date = body["end_date"].s();

            ProjectRepository::update_project(id, name, description, start_date, end_date);
            return crow::response(200, "Project updated");
        } 
        catch (const std::exception& e) 
        {
            return crow::response(500, "Failed to update project");
        }
    });


    CROW_ROUTE(app, "/projects/delete/<int>").methods("DELETE"_method)([](int id)
    {
        ProjectRepository::delete_project(id);
        return crow::response(200, "Project deleted");
    });
    // curl -X DELETE http://localhost:18080/projects/delete/1


    CROW_ROUTE(app, "/projects/deleteall").methods("DELETE"_method)([]()
    {
        CROW_LOG_INFO << "Delete all projects endpoint called";
        ProjectRepository::delete_all_projects();
        return crow::response(200, "All projects deleted");
    });
    // curl -X DELETE http://localhost:18080/projects/deleteall

    



    CROW_ROUTE(app, "/users")([&](const crow::request& req) 
    {
        crow::mustache::context context;
        populate_navbar_context(context, req, app);
        
        auto users = UserRepository::get_users();
        std::vector<crow::json::wvalue> user_list;

        for (const auto& user : users) {
            crow::json::wvalue u;
            u["id"]    = user.id;
            u["name"]  = user.name;
            u["email"] = user.email;
            u["role"]  = [&user]() {
                switch (user.role) {
                    case UserRole::ADMIN:           return "Admin";
                    case UserRole::PROJECT_MANAGER: return "Project Manager";
                    case UserRole::TEAM_MEMBER:     return "Team Member";
                    default:                        return "Unknown";
                }
            }();
            u["profile_picture"] = user.profile_picture;
            
            // Add role-specific flags for styling
            u["isAdmin"]          = (user.role == UserRole::ADMIN);
            u["isProjectManager"] = (user.role == UserRole::PROJECT_MANAGER);
            u["isTeamMember"]     = (user.role == UserRole::TEAM_MEMBER);
            
            user_list.push_back(std::move(u));
        }

        context["users"] = std::move(user_list);
        auto page = crow::mustache::load("users.html").render(context);
        return page;
    });


    CROW_ROUTE(app, "/users/create").methods("POST"_method)([](const crow::request& req)
    {
        auto body = crow::json::load(req.body);
        if (!body)
        {
            return crow::response(crow::status::BAD_REQUEST);
        }

        std::string name            = body["name"].s();
        std::string email           = body["email"].s();
        std::string password        = body["password"].s();
        UserRole role               = static_cast<UserRole>(body["role"].i());
        std::string profile_picture = body.has("profile_picture") ? std::string(body["profile_picture"].s()) : std::string("default.png");

        UserRepository::create_user(name, email, password, role, profile_picture);

        return crow::response(201, "User created");
    });
    // curl -X POST http://localhost:18080/users/create -d '{"name": "Test", "email": "test@test.com", "password": "test", "role": 1}'


    CROW_ROUTE(app, "/users/delete/<int>").methods("DELETE"_method)([](int id)
    {
        UserRepository::delete_user(id);
        return crow::response(200, "User deleted");
    });
    // curl -X DELETE http://localhost:18080/users/delete/1


    CROW_ROUTE(app, "/users/deleteall").methods("DELETE"_method)([]()
    {
        UserRepository::delete_all_users();
        return crow::response(200, "All users deleted");
    });
    // curl -X DELETE http://localhost:18080/users/deleteall

    

    

    // Add user to project
    CROW_ROUTE(app, "/projects/<int>/users/add").methods("POST"_method)([](const crow::request& req, int project_id) 
    {
        auto body = crow::json::load(req.body);
        if (!body) 
            return crow::response(400, "Invalid JSON body");
        
        int user_id = body["user_id"].i();
        try 
        {
            ProjectRepository::add_user_to_project(user_id, project_id);
            return crow::response(200, "User added to project");
        } 
        catch (const std::exception& e) 
        {
            return crow::response(500, "Failed to add user to project");
        }
    });
    // curl -X POST http://localhost:18080/projects/1/users/add -d '{"user_id": 1}'


    // Remove user from project
    CROW_ROUTE(app, "/projects/<int>/users/<int>/remove").methods("DELETE"_method)([](int project_id, int user_id) 
    {
        try 
        {
            ProjectRepository::remove_user_from_project(user_id, project_id);
            return crow::response(200, "User removed from project");
        } 
        catch (const std::exception& e) 
        {
            return crow::response(500, "Failed to remove user from project");
        }
    });
    // curl -X DELETE http://localhost:18080/projects/1/users/1/remove


    // Get project users
    CROW_ROUTE(app, "/projects/<int>/users")([](int project_id) 
    {
        auto users = ProjectRepository::get_project_users(project_id);
        crow::json::wvalue response;
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

        response["users"] = std::move(user_list);
        return response;
    });
    // curl -X GET http://localhost:18080/projects/1/users


    // Profile
    CROW_ROUTE(app, "/profile").methods("GET"_method)([&](const crow::request& req) -> crow::response
    {
        auto& session = app.get_context<Session>(req);
        auto username = session.get("user", "");

        // If user is not logged in, redirect to login page
        if (username.empty()) 
        {
            crow::response redirect(302);
            redirect.set_header("Location", "/login");
            return redirect;
        }

        // Get user data
        auto user = UserRepository::get_user_by_email(username);
        if (!user) 
        {
            crow::response redirect(302);
            redirect.set_header("Location", "/login");
            return redirect;
        }

        crow::mustache::context context;
        populate_navbar_context(context, req, app);

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
    });




    // Configure and run the application (on http://localhost:18080)
    app.port(18080)
       .multithreaded()
       .run();
}