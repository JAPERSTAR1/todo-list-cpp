#include "TodoList.h"

#include <exception>
#include <iostream>
#include <limits>
#include <optional>
#include <string>
#include <vector>

using namespace todo;

namespace {

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string prompt(const std::string& message) {
    std::cout << message;
    std::string value;
    std::getline(std::cin, value);
    return value;
}

int readInt(const std::string& message) {
    while (true) {
        std::cout << message;
        int value{};
        if (std::cin >> value) {
            clearInput();
            return value;
        }
        clearInput();
        std::cout << "Invalid number. Try again.\n";
    }
}

Priority readPriority() {
    while (true) {
        const std::string value = prompt("Priority (Low/Medium/High): ");
        try { return Task::priorityFromString(value); }
        catch (const std::exception& error) { std::cout << error.what() << '\n'; }
    }
}

void printTask(const Task& task) {
    std::cout << "\n[" << task.getId() << "] " << task.getTitle() << '\n'
              << "    Status: " << (task.isCompleted() ? "Completed" : "Pending") << '\n'
              << "    Priority: " << Task::priorityToString(task.getPriority()) << '\n'
              << "    Due date: " << (task.getDueDate().empty() ? "None" : task.getDueDate()) << '\n'
              << "    Description: " << (task.getDescription().empty() ? "None" : task.getDescription()) << '\n';
}

void printTasks(const std::vector<Task>& tasks) {
    if (tasks.empty()) {
        std::cout << "No tasks found.\n";
        return;
    }
    for (const Task& task : tasks) printTask(task);
}

void addTask(TodoList& list) {
    const std::string title = prompt("Title: ");
    if (title.empty()) { std::cout << "Title cannot be empty.\n"; return; }
    const std::string description = prompt("Description: ");
    const Priority priority = readPriority();
    const std::string dueDate = prompt("Due date (optional): ");
    const int id = list.addTask(title, description, priority, dueDate);
    list.save();
    std::cout << "Task created with ID " << id << ".\n";
}

void editTask(TodoList& list) {
    const int id = readInt("Task ID: ");
    if (!list.findTask(id)) { std::cout << "Task not found.\n"; return; }
    const std::string title = prompt("New title: ");
    const std::string description = prompt("New description: ");
    const Priority priority = readPriority();
    const std::string dueDate = prompt("New due date (optional): ");
    list.updateTask(id, title, description, priority, dueDate);
    list.save();
    std::cout << "Task updated.\n";
}

void filterTasks(const TodoList& list) {
    std::cout << "1. Pending\n2. Completed\n3. Low priority\n4. Medium priority\n5. High priority\n";
    const int choice = readInt("Filter: ");
    std::vector<Task> result;
    switch (choice) {
        case 1: result = list.filter(false, std::nullopt); break;
        case 2: result = list.filter(true, std::nullopt); break;
        case 3: result = list.filter(std::nullopt, Priority::Low); break;
        case 4: result = list.filter(std::nullopt, Priority::Medium); break;
        case 5: result = list.filter(std::nullopt, Priority::High); break;
        default: std::cout << "Invalid filter.\n"; return;
    }
    printTasks(result);
}

void showMenu() {
    std::cout << "\n========== TODO LIST ==========" << '\n'
              << "1. Add task\n2. List all tasks\n3. View task\n4. Edit task\n"
              << "5. Delete task\n6. Mark task as completed\n7. Mark task as pending\n"
              << "8. Search tasks\n9. Filter tasks\n0. Exit\n";
}

} // namespace

int main() {
    TodoList list;
    if (!list.load()) std::cerr << "Warning: Could not load saved tasks.\n";

    std::cout << "Welcome to Todo List CLI!\n";
    while (true) {
        showMenu();
        const int choice = readInt("Choose an option: ");
        switch (choice) {
            case 1: addTask(list); break;
            case 2: printTasks(list.getAllTasks()); break;
            case 3: {
                const Task* task = list.findTask(readInt("Task ID: "));
                if (task) printTask(*task); else std::cout << "Task not found.\n";
                break;
            }
            case 4: editTask(list); break;
            case 5: {
                const int id = readInt("Task ID: ");
                if (list.removeTask(id)) { list.save(); std::cout << "Task deleted.\n"; }
                else std::cout << "Task not found.\n";
                break;
            }
            case 6: {
                const int id = readInt("Task ID: ");
                if (list.completeTask(id)) { list.save(); std::cout << "Task completed.\n"; }
                else std::cout << "Task not found.\n";
                break;
            }
            case 7: {
                const int id = readInt("Task ID: ");
                if (list.reopenTask(id)) { list.save(); std::cout << "Task marked as pending.\n"; }
                else std::cout << "Task not found.\n";
                break;
            }
            case 8: printTasks(list.search(prompt("Search: "))); break;
            case 9: filterTasks(list); break;
            case 0: list.save(); std::cout << "Goodbye!\n"; return 0;
            default: std::cout << "Invalid option.\n";
        }
    }
}
