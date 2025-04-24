#include "TodoList.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <utility>

namespace todo {
namespace {

std::string sanitize(std::string value) {
    std::replace(value.begin(), value.end(), '\t', ' ');
    std::replace(value.begin(), value.end(), '\n', ' ');
    return value;
}

std::string lower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return value;
}

std::vector<std::string> split(const std::string& line) {
    std::vector<std::string> fields;
    std::stringstream stream(line);
    std::string field;
    while (std::getline(stream, field, '\t')) fields.push_back(field);
    return fields;
}

} // namespace

TodoList::TodoList(std::string storagePath)
    : storagePath_(std::move(storagePath)), nextId_(1) {}

int TodoList::addTask(std::string title, std::string description, Priority priority, std::string dueDate) {
    const int id = nextId_++;
    tasks_.emplace_back(id, sanitize(std::move(title)), sanitize(std::move(description)), priority,
                        sanitize(std::move(dueDate)));
    return id;
}

Task* TodoList::findTaskMutable(int id) {
    const auto it = std::find_if(tasks_.begin(), tasks_.end(), [id](const Task& task) {
        return task.getId() == id;
    });
    return it == tasks_.end() ? nullptr : &*it;
}

const Task* TodoList::findTask(int id) const {
    const auto it = std::find_if(tasks_.begin(), tasks_.end(), [id](const Task& task) {
        return task.getId() == id;
    });
    return it == tasks_.end() ? nullptr : &*it;
}

bool TodoList::removeTask(int id) {
    const auto it = std::remove_if(tasks_.begin(), tasks_.end(), [id](const Task& task) {
        return task.getId() == id;
    });
    if (it == tasks_.end()) return false;
    tasks_.erase(it, tasks_.end());
    return true;
}

bool TodoList::completeTask(int id) {
    Task* task = findTaskMutable(id);
    if (!task) return false;
    task->setCompleted(true);
    return true;
}

bool TodoList::reopenTask(int id) {
    Task* task = findTaskMutable(id);
    if (!task) return false;
    task->setCompleted(false);
    return true;
}

bool TodoList::updateTask(int id, std::string title, std::string description,
                          Priority priority, std::string dueDate) {
    Task* task = findTaskMutable(id);
    if (!task) return false;
    task->setTitle(sanitize(std::move(title)));
    task->setDescription(sanitize(std::move(description)));
    task->setPriority(priority);
    task->setDueDate(sanitize(std::move(dueDate)));
    return true;
}

std::vector<Task> TodoList::getAllTasks() const { return tasks_; }

std::vector<Task> TodoList::search(const std::string& query) const {
    const std::string needle = lower(query);
    std::vector<Task> result;
    for (const Task& task : tasks_) {
        if (lower(task.getTitle()).find(needle) != std::string::npos ||
            lower(task.getDescription()).find(needle) != std::string::npos) {
            result.push_back(task);
        }
    }
    return result;
}

std::vector<Task> TodoList::filter(std::optional<bool> completed,
                                   std::optional<Priority> priority) const {
    std::vector<Task> result;
    for (const Task& task : tasks_) {
        if (completed.has_value() && task.isCompleted() != *completed) continue;
        if (priority.has_value() && task.getPriority() != *priority) continue;
        result.push_back(task);
    }
    return result;
}

bool TodoList::load() {
    std::ifstream file(storagePath_);
    if (!file) return true;

    tasks_.clear();
    nextId_ = 1;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        const auto fields = split(line);
        if (fields.size() != 6) continue;
        try {
            const int id = std::stoi(fields[0]);
            Task task(id, fields[1], fields[2], Task::priorityFromString(fields[3]), fields[4]);
            task.setCompleted(fields[5] == "1");
            tasks_.push_back(std::move(task));
            nextId_ = std::max(nextId_, id + 1);
        } catch (...) {
            // Ignore malformed records and continue loading valid tasks.
        }
    }
    return true;
}

bool TodoList::save() const {
    const std::filesystem::path path(storagePath_);
    if (!path.parent_path().empty()) {
        std::error_code error;
        std::filesystem::create_directories(path.parent_path(), error);
        if (error) return false;
    }

    const std::string temporaryPath = storagePath_ + ".tmp";
    std::ofstream file(temporaryPath, std::ios::trunc);
    if (!file) return false;

    for (const Task& task : tasks_) {
        file << task.getId() << '\t' << sanitize(task.getTitle()) << '\t'
             << sanitize(task.getDescription()) << '\t'
             << Task::priorityToString(task.getPriority()) << '\t'
             << sanitize(task.getDueDate()) << '\t'
             << (task.isCompleted() ? "1" : "0") << '\n';
    }
    file.close();
    if (!file) return false;

    std::error_code error;
    std::filesystem::rename(temporaryPath, storagePath_, error);
    if (error) {
        std::filesystem::remove(storagePath_, error);
        error.clear();
        std::filesystem::rename(temporaryPath, storagePath_, error);
    }
    return !error;
}

std::size_t TodoList::size() const noexcept { return tasks_.size(); }

} // namespace todo
