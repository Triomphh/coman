#pragma once

#include "User.hpp"
#include <string>
#include <vector>

enum class TaskPriority 
{
    LOW,
    MEDIUM,
    HIGH
};

enum class TaskStatus
{
    TODO,
    IN_PROGRESS,
    DONE
};

class Task 
{
public:
    int id;
    std::string title;
    std::string description;
    std::vector<User> assignedTo;
    TaskPriority priority;
    TaskStatus status;
    std::string deadline;
};