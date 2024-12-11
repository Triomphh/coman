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

    // Helper method to create a test project
    Project createTestProject(const std::string& name = "Test Project",
                            const std::string& description = "A test project",
                            const std::string& start_date = "2024-01-01",
                            const std::string& end_date = "2024-12-31") {
        Project project;
        project.name = name;
        project.description = description;
        project.start_date = start_date;
        project.end_date = end_date;
        return project;
    }
};



// Test project creation and retrieval
TEST_F(ProjectRepositoryTest, CreateAndGetProject) 
{
    // Create a test project
    Project project = createTestProject();

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


// Test getting a non-existent project
TEST_F(ProjectRepositoryTest, GetNonExistentProject)
{
    auto retrieved = repo->get(1);
    EXPECT_FALSE(retrieved.has_value());
}


// Test updating a project
TEST_F(ProjectRepositoryTest, UpdateProject)
{
    // Create initial project
    Project project = createTestProject();
    repo->create(project);

    // Update the project
    Project updated_project = createTestProject("Updated Project", 
                                             "An updated project",
                                             "2024-02-01",
                                             "2024-11-30");
    updated_project.id = 1;
    repo->update(updated_project);

    // Verify the update
    auto retrieved = repo->get(1);
    ASSERT_TRUE(retrieved.has_value());
    EXPECT_EQ(retrieved->name, "Updated Project");
    EXPECT_EQ(retrieved->description, "An updated project");
    EXPECT_EQ(retrieved->start_date, "2024-02-01");
    EXPECT_EQ(retrieved->end_date, "2024-11-30");
}


// Test getting all projects
TEST_F(ProjectRepositoryTest, GetAllProjects)
{
    // Create multiple projects
    repo->create(createTestProject("Project 1", "First project"));
    repo->create(createTestProject("Project 2", "Second project"));
    repo->create(createTestProject("Project 3", "Third project"));

    // Get all projects
    auto projects = repo->get_all();

    // Verify the number of projects
    ASSERT_EQ(projects.size(), 3);

    // Verify the projects are in order
    EXPECT_EQ(projects[0].name, "Project 1");
    EXPECT_EQ(projects[1].name, "Project 2");
    EXPECT_EQ(projects[2].name, "Project 3");
}


// Test removing a project
TEST_F(ProjectRepositoryTest, RemoveProject)
{
    // Create a project
    repo->create(createTestProject());

    // Verify it exists
    ASSERT_TRUE(repo->get(1).has_value());

    // Remove the project
    repo->remove(1);

    // Verify it's gone
    EXPECT_FALSE(repo->get(1).has_value());
}


// Test removing all projects
TEST_F(ProjectRepositoryTest, RemoveAllProjects)
{
    // Create multiple projects
    repo->create(createTestProject("Project 1"));
    repo->create(createTestProject("Project 2"));
    repo->create(createTestProject("Project 3"));

    // Verify we have 3 projects
    ASSERT_EQ(repo->get_all().size(), 3);

    // Remove all projects
    repo->remove_all();

    // Verify all projects are gone
    EXPECT_EQ(repo->get_all().size(), 0);
}