#include "../third_party/crow_all.h"    // Using Crow framework to manage HTTP web services (taking care of routing, multithreading, etc...)

#include <string>



int main()
{
    // Define the Crow application
    crow::SimpleApp app;



    // Define differents endpoints
    CROW_ROUTE(app, "/")([]()
    {
        // auto page = crow::mustache::load_text("simplepage.html");
        // return page;
        return "Hello World!";
    });

    CROW_ROUTE(app, "/users")([]()
    {
        return "Here is the /users endpoint";
    });   



    // Configure and run the application (on http://127.0.0.1)
    app.bindaddr("127.0.0.1")
        .port(18080)
        .multithreaded()
        .run();
}
