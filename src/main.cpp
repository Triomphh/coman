#include "../third_party/crow_all.h"    // Using Crow framework to manage HTTP web services (taking care of routing, multithreading, etc...)
#include <SQLiteCpp/SQLiteCpp.h>
#include <SQLiteCpp/VariadicBind.h>
#include <string>


class Project
{
public:
    int id;
    std::string name;
    std::string description;
};

void create_project(const std::string& name, const std::string& description)
{
    SQLite::Database db("database.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    SQLite::Statement query(db, "INSERT INTO projects (name, description) VALUES (?, ?)");
    query.bind(1, name);
    query.bind(2, description);
    query.exec();
}

std::vector<Project> get_projects()
{
    SQLite::Database db("database.db", SQLite::OPEN_READONLY);
    SQLite::Statement query(db, "SELECT id, name, description FROM projects");

    std::vector<Project> projects;
    while (query.executeStep())
    {
        Project project;
        project.id = query.getColumn(0).getInt();
        project.name = query.getColumn(1).getString();
        project.description = query.getColumn(2).getString();
        projects.push_back(project);
    }
    return projects;
}

void update_project(int id, const std::string& name, const std::string& description)
{
    SQLite::Database db("database.db", SQLite::OPEN_READWRITE);
    SQLite::Statement query(db, "UPDATE projects SET name = ?, description = ? WHERE id = ?");
    query.bind(1, name);
    query.bind(2, description);
    query.bind(3, id);
    query.exec();
}

void delete_project(int id)
{
    SQLite::Database db("database.db", SQLite::OPEN_READWRITE);
    SQLite::Statement query(db, "DELETE FROM projects WHERE id = ?");
    query.bind(1, id);
    query.exec();
}

void delete_all_projects()
{
    SQLite::Database db("database.db", SQLite::OPEN_READWRITE);
    db.exec("DELETE FROM projects");
}

int main()
{
    // Define the Crow application
    crow::SimpleApp app;

    // app.loglevel(crow::LogLevel::Info);    // Show all logs bellow this level (see https://crowcpp.org/master/guides/logging/ for more info)


    // Initialize the SQLite database
    SQLite::Database db("database.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    db.exec("CREATE TABLE IF NOT EXISTS projects (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, description TEXT)");



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
        auto projects = get_projects();
        crow::mustache::context context;
        std::vector<crow::json::wvalue> project_list;
        
        for (const auto& project : projects) {
            crow::json::wvalue p;
            p["id"] = project.id;
            p["name"] = project.name;
            p["description"] = project.description;
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

        std::string name = body["name"].s();
        std::string description = body["description"].s();

        create_project(name, description);

        return crow::response(201, "Project created");
    });
    // curl -X POST http://localhost:18080/projects/create -d '{"name": "Test", "description": "Test description"}'

    CROW_ROUTE(app, "/projects/deleteall").methods("DELETE"_method)([]()
    {
        CROW_LOG_INFO << "Delete all projects endpoint called";
        delete_all_projects();
        return crow::response(200, "All projects deleted");
    });
    // curl -X DELETE http://localhost:18080/projects/deleteall



    CROW_ROUTE(app, "/users")([]()
    {
        CROW_LOG_INFO << "This endpoint isn't implemented yet";
        return crow::response( crow::status::NOT_IMPLEMENTED );     // Not implemented error code
    });   



    // Configure and run the application (on http://0.0.0.0:18080 (localhost))
    app.port(18080)
       .multithreaded()
       .run();
}
