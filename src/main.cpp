#include "include/crow_all.h"   // Using Crow framework to manage HTTP web services (taking care of routing, multithreading, etc...)
#include <string>



int main()
{
    // Define the Crow application
    crow::SimpleApp app;



    // Define differents endpoints
    CROW_ROUTE(app, "/")([]()
    {
        auto page = crow::mustache::load_text("simplepage.html");

        return page;
    });

    CROW_ROUTE(app, "/users")([]()
    {
        return "Here is the /users endpoint";
    });   



    // Set the port, set the app to run on multiple threads, and run the app on http://localhost:<port> (here http://localhost:18080)
    app.port(18080).multithreaded().run();
}
