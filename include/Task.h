#pragma once

#include <string>

namespace todo {

enum class Priority {
    Low,
    Medium,
    High
};

class Task {
public:
    Task(int id, std::string title, std::string description,
         Priority priority, std::string dueDate = "");

    int getId() const noexcept;
    const std::string& getTitle() const noexcept;
    const std::string& getDescription() const noexcept;
    Priority getPriority() const noexcept;
    const std::string& getDueDate() const noexcept;
    bool isCompleted() const noexcept;

    void setTitle(std::string title);
    void setDescription(std::string description);
    void setPriority(Priority priority) noexcept;
    void setDueDate(std::string dueDate);
    void setCompleted(bool completed) noexcept;

    static std::string priorityToString(Priority priority);
    static Priority priorityFromString(const std::string& value);

private:
    int id_;
    std::string title_;
    std::string description_;
    Priority priority_;
    std::string dueDate_;
    bool completed_;
};

} // namespace todo
