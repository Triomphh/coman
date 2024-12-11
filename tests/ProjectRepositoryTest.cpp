#include <gtest/gtest.h>
#include <SQLiteCpp/SQLiteCpp.h>
#include "../include/repositories/ProjectRepository.hpp"
#include "../include/models/Project.hpp"



class ProjectRepositoryTest : public ::testing::Test 
{
protected:
    std::shared_ptr<SQLite::Database> db;
    std::unique_ptr<ProjectRepository> repo;

    void SetUp() override 
    {
        // Create an in-memory database for testing
        db = std::make_shared<SQLite::Database>(":memory:", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        
        // Create the projects table
        db->exec("CREATE TABLE projects ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "name TEXT NOT NULL,"
                "description TEXT,"
                "start_date TEXT,"
                "end_date TEXT"
                ")");
        
        repo = std::make_unique<ProjectRepository>(db);
    }

    void TearDown() override 
    {
        // Automatic cleanup (database in memory)
    }
};


// Project creation test
TEST_F(ProjectRepositoryTest, CreateAndGetProject) 
{
    // Create a test project
    Project project;
    project.name = "Test Project";
    project.description = "A test project";
    project.start_date = "2024-01-01";
    project.end_date = "2024-12-31";

    // Create the project in the database
    repo->create(project);

    // Get the project with ID 1 (first project)
    auto retrieved = repo->get(1);

    // Verify the project is correct
    ASSERT_TRUE(retrieved.has_value());
    EXPECT_EQ(retrieved->name, "Test Project");
    EXPECT_EQ(retrieved->description, "A test project");
    EXPECT_EQ(retrieved->start_date, "2024-01-01");
    EXPECT_EQ(retrieved->end_date, "2024-12-31");
} 