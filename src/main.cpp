#include "../third_party/crow_all.h"    // Using Crow framework to manage HTTP web services (taking care of routing, multithreading, etc...)

#include <string>



int main()
{
    // Define the Crow application
    crow::SimpleApp app;

    // app.loglevel(crow::LogLevel::Info);    // Show all logs bellow this level (see https://crowcpp.org/master/guides/logging/ for more info)

    

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
        auto page = crow::mustache::load("projects.html").render();
        return page;
    });

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
