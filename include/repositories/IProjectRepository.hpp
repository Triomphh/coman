#pragma once

#include "../models/Project.hpp"
#include <optional>
#include <vector>


class IProjectRepository
{
public:
    virtual ~IProjectRepository() = default;

    // CRUD operations
    virtual void create(const Project& project) = 0;
    virtual std::optional<Project> get(int id) = 0;
    virtual std::vector<Project> get_all() = 0;
    virtual void update(const Project& project) = 0;
    virtual void remove(int id) = 0;
    virtual void remove_all() = 0;
};