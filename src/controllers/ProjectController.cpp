#include "../../include/controllers/ProjectController.hpp"
#include "../../include/utils/ContextUtils.hpp"


ProjectController::ProjectController(
    std::shared_ptr<IProjectService> project_service, 
    std::shared_ptr<IProjectMembershipService> project_membership_service, 
    std::shared_ptr<TaskController> task_controller,
    std::shared_ptr<IUserService> user_service
) : project_service(std::move(project_service)), 
    project_membership_service(std::move(project_membership_service)), 
    task_controller(std::move(task_controller)),
    user_service(std::move(user_service)) {}


void ProjectController::register_routes(crow::App<crow::CookieParser, crow::SessionMiddleware<crow::FileStore>>& app)
{
    CROW_ROUTE(app, "/projects")
        ([this](const crow::request& req) 
        { 
            return handle_get_all_projects(req); 
        });

    CROW_ROUTE(app, "/projects/<int>")
        ([this](const crow::request& req, int id) 
        { 
            return handle_get_project(req, id); 
        });

    CROW_ROUTE(app, "/projects/create")
        .methods("POST"_method)
        ([this](const crow::request& req) 
        { 
            return handle_create_project(req); 
        });

    CROW_ROUTE(app, "/projects/<int>/update")
        .methods("PUT"_method)
        ([this](const crow::request& req, int id) 
        { 
            return handle_update_project(req, id); 
        });

    CROW_ROUTE(app, "/projects/delete/<int>")
        .methods("DELETE"_method)
        ([this](const crow::request& req, int id) 
        { 
            return handle_delete_project(req, id); 
        });

    CROW_ROUTE(app, "/projects/deleteall")
        .methods("DELETE"_method)
        ([this](const crow::request& req) 
        { 
            return handle_delete_all_projects(req); 
        });
}



crow::response ProjectController::handle_create_project(const crow::request& req)
{
    try
    {
        auto body = crow::json::load(req.body);
        if (!body)
        {
            return crow::response(400, "Invalid JSON body");
        }

        auto project = project_service->create_project(
            body["name"].s(),
            body["description"].s(),
            body["start_date"].s(),
            body["end_date"].s()
        );

        return crow::response(201, "Project created");
    }
    catch (const std::exception& e)
    {
        return crow::response(500, "Internal server error");
    }
}


crow::response ProjectController::handle_get_project(const crow::request& req, int id)
{
    try
    {
        crow::mustache::context context;
        utils::ContextUtils::populate_navbar_context(context, req, user_service);

        auto project = project_service->get_project(id);
        if (!project)
        {
            return crow::response(404, "Project not found");
        }

        // Add project details
        context["id"] = project->id;
        context["name"] = project->name;
        context["description"] = project->description;
        context["start_date"] = project->start_date;
        context["end_date"] = project->end_date;

        // Get project users
        auto project_users = project_membership_service->get_project_users(id);
        std::vector<crow::json::wvalue> user_list;
        for (const auto& user : project_users)
        {
            crow::json::wvalue u;
            u["name"]            = user.name;
            u["profile_picture"] = user.profile_picture;
            user_list.push_back(std::move(u));
        }
        context["users"] = std::move(user_list);

        // Get tasks using TaskController
        auto tasks_response = task_controller->handle_get_tasks(req, id);
        if (tasks_response.code >= 400)
        {
            return tasks_response;  // Return error if task fetch fails
        }
        
        // Parse the tasks response and add to context
        auto tasks_json = crow::json::load(tasks_response.body);
        if (tasks_json)
        {
            context["tasks"] = std::move(tasks_json["tasks"]);
        }

        auto page = crow::mustache::load("project_details.html").render(context);
        return crow::response(page);
    }
    catch (const std::exception& e)
    {
        return crow::response(500, std::string("Internal server error: ") + e.what());
    }
}



crow::response ProjectController::handle_get_all_projects(const crow::request& req)
{
    try
    {
        crow::mustache::context context;
        utils::ContextUtils::populate_navbar_context(context, req, user_service);

        auto projects = project_service->get_all_projects();
        std::vector<crow::json::wvalue> project_list;

        for (const auto& project : projects)
        {
            crow::json::wvalue p;
            p["id"]            = project.id;
            p["name"]          = project.name;
            p["description"]   = project.description;
            p["start_date"]    = project.start_date;
            p["end_date"]      = project.end_date;

            // Get users for this project
            auto project_users = project_membership_service->get_project_users(project.id);
            std::vector<crow::json::wvalue> user_avatars;
            for (const auto& user : project_users)
            {
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
        return crow::response(page);
    }
    catch (const std::exception& e)
    {
        return crow::response(500, "Internal server error");
    }
}


crow::response ProjectController::handle_update_project(const crow::request& req, int id)
{
    try
    {
        auto body = crow::json::load(req.body);
        if (!body)
        {
            return crow::response(400, "Invalid JSON body");
        }

        project_service->update_project(
            id, 
            body["name"].s(),
            body["description"].s(),
            body["start_date"].s(),
            body["end_date"].s()
        );

        return crow::response(200, "Project updated");
    }
    catch (const std::exception& e)
    {
        return crow::response(500, "Internal server error");
    }
}


crow::response ProjectController::handle_delete_project(const crow::request& req, int id)
{
    try
    {
        project_service->delete_project(id);
        return crow::response(200, "Project deleted");
    }
    catch (const std::exception& e)
    {
        return crow::response(500, "Internal server error");
    }
}


crow::response ProjectController::handle_delete_all_projects(const crow::request& req)
{
    try
    {
        project_service->delete_all_projects();
        return crow::response(200, "All projects deleted");
    }
    catch (const std::exception& e)
    {
        return crow::response(500, "Internal server error");
    }
}