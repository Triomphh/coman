#include "../include/DatabaseManager.hpp"



DatabaseManager* DatabaseManager::instance = nullptr;   // Initialize the static instance pointer to a nullptr (required for Singleton pattern)


void DatabaseManager::initializeTables() {
    // Projects table
    db->exec("CREATE TABLE IF NOT EXISTS projects ("
             "id INTEGER PRIMARY KEY AUTOINCREMENT, "
             "name TEXT, "
             "description TEXT, "
             "start_date DATE, "
             "end_date DATE"
             ")"
    );

    // Tasks table
    db->exec("CREATE TABLE IF NOT EXISTS tasks ("
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

    // Users table
    db->exec("CREATE TABLE IF NOT EXISTS users ("
             "id INTEGER PRIMARY KEY AUTOINCREMENT, "
             "name TEXT, "
             "email TEXT, "
             "hashed_password TEXT, "
             "role TEXT, "
             "profile_picture TEXT DEFAULT 'default.png'"
             ")"
    );

    // Junction table for many-to-many relationship between users and projects
    db->exec("CREATE TABLE IF NOT EXISTS work_on_project ("
             "user_id INTEGER, "
             "project_id INTEGER, "
             "PRIMARY KEY (user_id, project_id), "
             "FOREIGN KEY (user_id) REFERENCES users(id), "
             "FOREIGN KEY (project_id) REFERENCES projects(id)"
             ")"
    );

    // Junction table for many-to-many relationship between users and tasks
    db->exec("CREATE TABLE IF NOT EXISTS work_on_task ("
             "user_id INTEGER, "
             "task_id INTEGER, "
             "PRIMARY KEY (user_id, task_id), "
             "FOREIGN KEY (user_id) REFERENCES users(id), "
             "FOREIGN KEY (task_id) REFERENCES tasks(id)"
             ")"
    );
} 