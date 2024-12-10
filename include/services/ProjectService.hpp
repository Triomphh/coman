#pragma once

#include "IProjectService.hpp"
#include "../repositories/IProjectRepository.hpp"
#include <memory>


class ProjectService : public IProjectService
{
private:
    std::shared_ptr<IProjectRepository> repository;

public:
    explicit ProjectService(std::shared_ptr<IProjectRepository> repo);
    ~ProjectService() override = default;

    Project create_project(const std::string& name, const std::string& description, const std::string& start_date, const std::string& end_date) override;
    std::optional<Project> get_project(int id) override;
    std::vector<Project> get_all_projects() override;
    void update_project(int id, const std::string& name, const std::string& description, const std::string& start_date, const std::string& end_date) override;
    void delete_project(int id) override;
    void delete_all_projects() override;
};