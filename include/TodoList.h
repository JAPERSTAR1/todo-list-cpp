#pragma once

#include "Task.h"

#include <optional>
#include <string>
#include <vector>

namespace todo {

class TodoList {
public:
    explicit TodoList(std::string storagePath = "data/tasks.db");

    int addTask(std::string title, std::string description,
                Priority priority, std::string dueDate = "");
    bool removeTask(int id);
    bool completeTask(int id);
    bool reopenTask(int id);
    bool updateTask(int id, std::string title, std::string description,
                    Priority priority, std::string dueDate);

    const Task* findTask(int id) const;
    std::vector<Task> getAllTasks() const;
    std::vector<Task> search(const std::string& query) const;
    std::vector<Task> filter(std::optional<bool> completed,
                             std::optional<Priority> priority) const;

    bool load();
    bool save() const;
    std::size_t size() const noexcept;

private:
    Task* findTaskMutable(int id);

    std::vector<Task> tasks_;
    std::string storagePath_;
    int nextId_;
};

} // namespace todo
