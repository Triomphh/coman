#pragma once

#include <SQLiteCpp/SQLiteCpp.h>
#include <memory>


// DatabaseManager class implements the Singleton pattern 
// to ensure only one instance of the database is created
class DatabaseManager 
{
private:
    static DatabaseManager* instance;
    std::unique_ptr<SQLite::Database> db;
    
    // Private constructor to prevent direct construction
    DatabaseManager() 
    {
        db = std::make_unique<SQLite::Database>("data/database.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        initializeTables();
    }

    void initializeTables();

public:
    // Prevent copying and assignment (singleton pattern)
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;
    
    static DatabaseManager* getInstance() 
    {
        if (instance == nullptr) {
            instance = new DatabaseManager();
        }
        return instance;
    }
    
    SQLite::Database* getDatabase() 
    {
        return db.get();
    }
}; 