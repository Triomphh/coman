#include "../third_party/crow_all.h"    // Using Crow framework to manage HTTP web services (taking care of routing, multithreading, etc...)
#include "../include/ProjectRepository.hpp"
#include "../include/UserRepository.hpp"
#include <SQLiteCpp/SQLiteCpp.h>

int main()
{
    // Define the Crow application
    crow::SimpleApp app;

    // app.loglevel(crow::LogLevel::Info);    // Show all logs bellow this level (see https://crowcpp.org/master/guides/logging/ for more info)


    // Initialize the SQLite database
    SQLite::Database db("database.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    db.exec("CREATE TABLE IF NOT EXISTS projects ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "name TEXT, "
            "description TEXT, "
            "start_date DATE, "
            "end_date DATE"
            ")"
    );

    db.exec("CREATE TABLE IF NOT EXISTS tasks ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "title TEXT, "
            "description TEXT, "
            "priority TEXT, "
            "status TEXT, "
            "deadline DATE, "
            "project_id INTEGER, "
            "FOREIGN KEY (project_id) REFERENCES projects(id)"
            ")"
    );

    db.exec("CREATE TABLE IF NOT EXISTS users ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "name TEXT, "
            "email TEXT, "
            "hashed_password TEXT, "
            "role TEXT"
            ")"
    );



    // Define differents endpoints
    CROW_ROUTE(app, "/")([]()
    {
        crow::mustache::context context; 

        context["name"] = "Pablo";      // Just a JSON syntax: {"name" : "Pablo"}

        auto page = crow::mustache::load("dashboard.html").render(context);
        return page;
    });



    CROW_ROUTE(app, "/projects")([]()
    {
        auto projects = ProjectRepository::get_projects();
        crow::mustache::context context;
        std::vector<crow::json::wvalue> project_list;
        
        for (const auto& project : projects) {
            crow::json::wvalue p;
            p["id"]            = project.id;
            p["name"]          = project.name;
            p["description"]   = project.description;
            p["start_date"]    = project.start_date;
            p["end_date"]      = project.end_date;
            project_list.push_back(std::move(p));
        }
        
        context["projects"] = std::move(project_list);
        auto page = crow::mustache::load("projects.html").render(context);
        return page;
    });

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



    CROW_ROUTE(app, "/users")([]()
    {
        auto users = UserRepository::get_users();
        crow::mustache::context context;
        std::vector<crow::json::wvalue> user_list;

        for (const auto& user : users) {
            crow::json::wvalue u;
            u["id"] = user.id;
            u["name"] = user.name;
            u["email"] = user.email;
            u["role"] = [&user]() {
                switch (user.role) {
                    case UserRole::ADMIN: return "Admin";
                    case UserRole::PROJECT_MANAGER: return "Project Manager";
                    case UserRole::TEAM_MEMBER: return "Team Member";
                    default: return "Unknown";
                }
            }();
            user_list.push_back(std::move(u));
        }

        context["users"] = std::move(user_list);
        return context;
    });   

    CROW_ROUTE(app, "/users/create").methods("POST"_method)([](const crow::request& req)
    {
        auto body = crow::json::load(req.body);
        if (!body)
        {
            return crow::response(crow::status::BAD_REQUEST);
        }

        std::string name = body["name"].s();
        std::string email = body["email"].s();
        std::string password = body["password"].s();
        UserRole role = static_cast<UserRole>(body["role"].i());

        UserRepository::create_user(name, email, password, role);

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



    // Configure and run the application (on http://0.0.0.0:18080 (localhost))
    app.port(18080)
       .multithreaded()
       .run();
}
