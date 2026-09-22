#include "TodoList.h"

#include <cassert>
#include <cstdio>
#include <iostream>

using namespace todo;

int main() {
    const char* testFile = "test_tasks.db";
    std::remove(testFile);

    TodoList list(testFile);
    const int id = list.addTask("Learn C++", "Study classes and vectors", Priority::High, "2026-10-01");

    assert(id == 1);
    assert(list.size() == 1);
    assert(list.findTask(id) != nullptr);
    assert(list.findTask(id)->getPriority() == Priority::High);

    assert(list.completeTask(id));
    assert(list.findTask(id)->isCompleted());
    assert(list.reopenTask(id));
    assert(!list.findTask(id)->isCompleted());

    assert(list.updateTask(id, "Learn Modern C++", "Practice C++17", Priority::Medium, "2026-10-02"));
    assert(list.findTask(id)->getTitle() == "Learn Modern C++");

    assert(list.save());

    TodoList loaded(testFile);
    assert(loaded.load());
    assert(loaded.size() == 1);
    assert(loaded.findTask(1)->getTitle() == "Learn Modern C++");

    assert(loaded.removeTask(1));
    assert(loaded.size() == 0);

    std::remove(testFile);
    std::cout << "All tests passed.\n";
    return 0;
}
