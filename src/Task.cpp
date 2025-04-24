#include "Task.h"

#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <utility>

namespace todo {

Task::Task(int id, std::string title, std::string description,
           Priority priority, std::string dueDate)
    : id_(id), title_(std::move(title)), description_(std::move(description)),
      priority_(priority), dueDate_(std::move(dueDate)), completed_(false) {}

int Task::getId() const noexcept { return id_; }
const std::string& Task::getTitle() const noexcept { return title_; }
const std::string& Task::getDescription() const noexcept { return description_; }
Priority Task::getPriority() const noexcept { return priority_; }
const std::string& Task::getDueDate() const noexcept { return dueDate_; }
bool Task::isCompleted() const noexcept { return completed_; }

void Task::setTitle(std::string title) { title_ = std::move(title); }
void Task::setDescription(std::string description) { description_ = std::move(description); }
void Task::setPriority(Priority priority) noexcept { priority_ = priority; }
void Task::setDueDate(std::string dueDate) { dueDate_ = std::move(dueDate); }
void Task::setCompleted(bool completed) noexcept { completed_ = completed; }

std::string Task::priorityToString(Priority priority) {
    switch (priority) {
        case Priority::Low: return "Low";
        case Priority::Medium: return "Medium";
        case Priority::High: return "High";
    }
    return "Medium";
}

Priority Task::priorityFromString(const std::string& value) {
    std::string normalized = value;
    std::transform(normalized.begin(), normalized.end(), normalized.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    if (normalized == "low") return Priority::Low;
    if (normalized == "medium") return Priority::Medium;
    if (normalized == "high") return Priority::High;
    throw std::invalid_argument("Invalid priority. Use Low, Medium, or High.");
}

} // namespace todo
