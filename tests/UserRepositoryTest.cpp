#include <gtest/gtest.h>
#include <SQLiteCpp/SQLiteCpp.h>
#include "../include/repositories/UserRepository.hpp"
#include "../include/models/User.hpp"



class UserRepositoryTest : public ::testing::Test 
{
protected:
    std::shared_ptr<SQLite::Database> db;
    std::unique_ptr<UserRepository> repo;

    void SetUp() override 
    {
        // Create an in-memory database for testing
        db = std::make_shared<SQLite::Database>(":memory:", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        
        // Create the users table
        db->exec("CREATE TABLE users ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "name TEXT NOT NULL,"
                "email TEXT NOT NULL UNIQUE,"
                "hashed_password TEXT NOT NULL,"
                "role INTEGER NOT NULL,"
                "profile_picture TEXT"
                ")");
        
        repo = std::make_unique<UserRepository>(db);
    }

    void TearDown() override 
    {
        // Automatic cleanup (database in memory)
    }

    // Helper function to create a test user
    User createTestUser(const std::string& name = "Test",
                        const std::string& email = "test@test.com",
                        const std::string& password = "xxx",
                        UserRole role = UserRole::TEAM_MEMBER,
                        const std::string& profile_picture = "default.jpg") 
    {
        User user;
        user.name = name;
        user.email = email;
        user.hashed_password = password;
        user.role = role;
        user.profile_picture = profile_picture;
        return user;
    }
};



// Test user creation and retrieval
TEST_F(UserRepositoryTest, CreateAndGetUser) 
{
    User user = createTestUser();
    repo->create(user);

    auto retrieved = repo->get(1);

    ASSERT_TRUE(retrieved.has_value());
    EXPECT_EQ(retrieved->name, "Test");
    EXPECT_EQ(retrieved->email, "test@test.com");
    EXPECT_EQ(retrieved->hashed_password, "xxx");
    EXPECT_EQ(retrieved->role, UserRole::TEAM_MEMBER);
    EXPECT_EQ(retrieved->profile_picture, "default.jpg");
}


// Test getting a non-existent user
TEST_F(UserRepositoryTest, GetNonExistentUser)
{
    auto retrieved = repo->get(1);
    EXPECT_FALSE(retrieved.has_value());
}


// Test getting user by email
TEST_F(UserRepositoryTest, GetUserByEmail)
{
    User user = createTestUser();
    repo->create(user);

    auto retrieved = repo->get_by_email("test@test.com");

    ASSERT_TRUE(retrieved.has_value());
    EXPECT_EQ(retrieved->name, "Test");
    EXPECT_EQ(retrieved->email, "test@test.com");
}


// Test getting non-existent user by email
TEST_F(UserRepositoryTest, GetNonExistentUserByEmail)
{
    auto retrieved = repo->get_by_email("nonexistent@test.com");
    EXPECT_FALSE(retrieved.has_value());
}


// Test updating a user
TEST_F(UserRepositoryTest, UpdateUser)
{
    User user = createTestUser();
    repo->create(user);

    // Update the user
    User updated_user = createTestUser("Updated User", 
                                  "updated@test.com",
                                  "nouveaumdp",
                                  UserRole::ADMIN,
                                  "updated.jpg");
    updated_user.id = 1;
    repo->update(updated_user);

    // Verify the update
    auto retrieved = repo->get(1);
    ASSERT_TRUE(retrieved.has_value());
    EXPECT_EQ(retrieved->name, "Updated User");
    EXPECT_EQ(retrieved->email, "updated@test.com");
    EXPECT_EQ(retrieved->hashed_password, "nouveaumdp");
    EXPECT_EQ(retrieved->role, UserRole::ADMIN);
    EXPECT_EQ(retrieved->profile_picture, "updated.jpg");
}


// Test getting all users
TEST_F(UserRepositoryTest, GetAllUsers)
{
    repo->create(createTestUser("User 1", "user1@test.com"));
    repo->create(createTestUser("User 2", "user2@test.com"));
    repo->create(createTestUser("User 3", "user3@test.com"));

    auto users = repo->get_all();

    ASSERT_EQ(users.size(), 3);
    EXPECT_EQ(users[0].name, "User 1");
    EXPECT_EQ(users[1].name, "User 2");
    EXPECT_EQ(users[2].name, "User 3");
}


// Test removing a user
TEST_F(UserRepositoryTest, RemoveUser)
{
    repo->create(createTestUser());

    ASSERT_TRUE(repo->get(1).has_value());

    repo->remove(1);

    EXPECT_FALSE(repo->get(1).has_value());
}


// Test removing all users
TEST_F(UserRepositoryTest, RemoveAllUsers)
{
    repo->create(createTestUser("User 1", "user1@test.com"));
    repo->create(createTestUser("User 2", "user2@test.com"));
    repo->create(createTestUser("User 3", "user3@test.com"));

    ASSERT_EQ(repo->get_all().size(), 3);

    repo->remove_all();

    EXPECT_EQ(repo->get_all().size(), 0);
}


// Test unique email constraint
TEST_F(UserRepositoryTest, UniqueEmailConstraint)
{
    repo->create(createTestUser("User 1", "same@test.com"));

    User duplicate = createTestUser("User 2", "same@test.com");
    
    EXPECT_THROW(repo->create(duplicate), SQLite::Exception);
}


// Test creating user with empty profile picture
TEST_F(UserRepositoryTest, CreateUserWithEmptyProfilePicture)
{
    User user = createTestUser("Test User", "test@test.com", "hashedpass", UserRole::TEAM_MEMBER, "");
    repo->create(user);

    auto retrieved = repo->get(1);
    ASSERT_TRUE(retrieved.has_value());
    EXPECT_TRUE(retrieved->profile_picture.empty());
}


// Test different user roles
TEST_F(UserRepositoryTest, UserRoles)
{
    repo->create(createTestUser("Admin", "admin@test.com", "pass", UserRole::ADMIN));
    repo->create(createTestUser("User", "user@test.com", "pass", UserRole::TEAM_MEMBER));

    auto admin = repo->get_by_email("admin@test.com");
    auto user = repo->get_by_email("user@test.com");

    ASSERT_TRUE(admin.has_value());
    ASSERT_TRUE(user.has_value());
    EXPECT_EQ(admin->role, UserRole::ADMIN);
    EXPECT_EQ(user->role, UserRole::TEAM_MEMBER);
} 