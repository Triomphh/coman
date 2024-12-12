#include "../third_party/crow_all.h"
#include "../include/DatabaseManager.hpp"
#include "../include/controllers/ProjectController.hpp"
#include "../include/controllers/TaskController.hpp"
#include "../include/controllers/UserController.hpp"
#include "../include/controllers/DashboardController.hpp"
#include "../include/controllers/AuthenticationController.hpp"
#include "../include/controllers/ProjectMembershipController.hpp"
#include "../include/controllers/DocumentationController.hpp"
#include "../include/services/ProjectService.hpp"
#include "../include/services/TaskService.hpp"
#include "../include/services/UserService.hpp"
#include "../include/services/ProjectMembershipService.hpp"
#include "../include/services/AuthenticationService.hpp"
#include "../include/repositories/ProjectRepository.hpp"
#include "../include/repositories/TaskRepository.hpp"
#include "../include/repositories/UserRepository.hpp"
#include "../include/repositories/ProjectMembershipRepository.hpp"



int main() 
{
    // Define the session middleware
    using Session = crow::SessionMiddleware<crow::FileStore>;

    // Define the Crow application
    crow::App<crow::CookieParser, Session> app{Session{crow::FileStore("./sessions")}};

    // Initialize database
    DatabaseManager::getInstance();

    // Get database instance and wrap in shared_ptr
    auto db = std::shared_ptr<SQLite::Database>(DatabaseManager::getInstance()->getDatabase());

    // Create repositories with database instance
    auto project_repo = std::make_shared<ProjectRepository>(db);
    auto task_repo = std::make_shared<TaskRepository>(db);
    auto user_repo = std::make_shared<UserRepository>(db);
    auto project_membership_repo = std::make_shared<ProjectMembershipRepository>(db);

    // Create services
    auto project_service = std::make_shared<ProjectService>(project_repo);
    auto task_service = std::make_shared<TaskService>(task_repo);
    auto user_service = std::make_shared<UserService>(user_repo);
    auto project_membership_service = std::make_shared<ProjectMembershipService>(project_membership_repo);

    // Create controllers
    auto task_controller = std::make_shared<TaskController>(task_service);
    auto project_controller = std::make_shared<ProjectController>(
        project_service,
        project_membership_service,
        task_controller,
        user_service
    );
    auto user_controller = std::make_shared<UserController>(user_service);
    auto dashboard_controller = std::make_shared<DashboardController>(
        project_service,
        task_service,
        user_service
    );
    auto project_membership_controller = std::make_shared<ProjectMembershipController>(project_membership_service);
    // TODO: AuthenticationService
    auto auth_service = std::make_shared<AuthenticationService>(user_repo);
    auto authentication_controller = std::make_shared<AuthenticationController>(
        auth_service,
        user_service,
        app
    );
    auto documentation_controller = std::make_shared<DocumentationController>();

    // Register routes
    dashboard_controller->register_routes(app);
    project_controller->register_routes(app);
    task_controller->register_routes(app);
    user_controller->register_routes(app);
    authentication_controller->register_routes(app);
    project_membership_controller->register_routes(app);
    documentation_controller->register_routes(app);

    // Configure and run the application
    app.port(18080)
       .multithreaded()
       .run();

    return 0;
}