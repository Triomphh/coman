#include "include/crow_all.h"   // Using Crow framework to manage HTTP web services (taking care of routing, multithreading, etc...)



int main()
{
    // Define the Crow application
    crow::SimpleApp app;

    // Define the endpoint at the root directory
    CROW_ROUTE(app, "/")([]()
    {
        return "Hello world";
    });

    CROW_ROUTE(app, "/users")([]()
    {
        return "Here is the /users endpoint";
    });   

    // Set the port, set the app to run on multiple threads, and run the app on http://localhost:<port> (here http://localhost:18080)
    app.port(18080).multithreaded().run();
}
