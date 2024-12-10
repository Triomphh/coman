#include "../../include/controllers/TaskController.hpp"



TaskController::TaskController(std::shared_ptr<ITaskService> task_service)
    : task_service(std::move(task_service)) {}


void TaskController::register_routes(crow::App<crow::CookieParser, crow::SessionMiddleware<crow::FileStore>>& app)
{
    CROW_ROUTE(app, "/projects/<int>/tasks/create")
        .methods("POST"_method)
        ([this](const crow::request& req, int project_id) 
        { 
            return handle_create_task(req, project_id); 
        });

    CROW_ROUTE(app, "/projects/<int>/tasks")
        ([this](const crow::request& req, int project_id) 
        { 
            return handle_get_tasks(req, project_id); 
        });

    CROW_ROUTE(app, "/projects/<int>/tasks/<int>/update")
        .methods("PUT"_method)
        ([this](const crow::request& req, int project_id, int task_id) 
        { 
            return handle_update_task(req, project_id, task_id); 
        });

    CROW_ROUTE(app, "/projects/<int>/tasks/<int>/delete")
        .methods("DELETE"_method)
        ([this](const crow::request& req, int project_id, int task_id) 
        { 
            return handle_delete_task(req, project_id, task_id); 
        });

    CROW_ROUTE(app, "/projects/tasks/deleteall")
        .methods("DELETE"_method)
        ([this](const crow::request& req) 
        { 
            return handle_delete_all_tasks(req); 
        });
}



crow::response TaskController::handle_create_task(const crow::request& req, int project_id)
{
    try
    {
        auto body = crow::json::load(req.body);
        if (!body)
        {
            return crow::response(400, "Invalid JSON body");
        }

        task_service->create_task(
            body["title"].s(), 
            body["description"].s(), 
            static_cast<TaskPriority>(body["priority"].i()), 
            TaskStatus::TODO, 
            body["deadline"].s(), 
            project_id
        );

        return crow::response(201, "Task created");
    }
    catch (const std::exception& e)
    {
        return crow::response(500, "Internal server error: " + std::string(e.what()));
    }
}


crow::response TaskController::handle_get_tasks(const crow::request& req, int project_id)
{
    try
    {
        auto tasks = task_service->get_all_tasks_by_project(project_id);
        std::vector<crow::json::wvalue> task_list;

        for (const auto& task : tasks)
        {
            crow::json::wvalue t;
            t["id"]            = task.id;
            t["title"]         = task.title;
            t["description"]   = task.description;
            t["deadline"]      = task.deadline;
            t["status"]        = static_cast<int>(task.status);

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

        crow::json::wvalue response;
        response["tasks"] = std::move(task_list);
        return crow::response(200, response);
    }
    catch (const std::exception& e)
    {
        return crow::response(500, "Internal server error: " + std::string(e.what()));
    }
}


crow::response TaskController::handle_update_task(const crow::request& req, int project_id, int task_id)
{
    try
    {
        auto body = crow::json::load(req.body);
        if (!body)
        {
            return crow::response(400, "Invalid JSON body");
        }

        task_service->update_task(
            task_id, 
            body["title"].s(), 
            body["description"].s(), 
            static_cast<TaskPriority>(body["priority"].i()), 
            static_cast<TaskStatus>(body["status"].i()), 
            body["deadline"].s(), 
            project_id
        );

        return crow::response(200, "Task updated");
    }
    catch (const std::exception& e)
    {
        return crow::response(500, "Internal server error: " + std::string(e.what()));
    }
}


crow::response TaskController::handle_delete_task(const crow::request& req, int project_id, int task_id)
{
    try
    {
        task_service->delete_task(task_id);
        return crow::response(204);
    }
    catch (const std::exception& e)
    {
        return crow::response(500, "Internal server error: " + std::string(e.what()));
    }
}


crow::response TaskController::handle_delete_all_tasks(const crow::request& req)
{
    try
    {
        task_service->delete_all_tasks();
        return crow::response(204);
    }
    catch (const std::exception& e)
    {
        return crow::response(500, "Internal server error: " + std::string(e.what()));
    }
}