# Todo List CLI

A complete command-line Todo List application written in modern C++17.

## Features

- Create, edit, delete, and view tasks
- Mark tasks as completed or pending
- Task priorities: Low, Medium, High
- Optional due dates
- Search tasks by title or description
- Filter by status and priority
- Persistent local storage
- Automatic task ID management
- Clean object-oriented project structure
- CMake build configuration
- Basic automated tests

## Project Structure

```text
todo-list-cpp/
├── CMakeLists.txt
├── README.md
├── .gitignore
├── include/
│   ├── Task.h
│   └── TodoList.h
├── src/
│   ├── Task.cpp
│   ├── TodoList.cpp
│   └── main.cpp
├── tests/
│   └── TodoListTests.cpp
└── data/
    └── .gitkeep
```

## Requirements

- C++17 compatible compiler
- CMake 3.16 or newer

## Build

```bash
cmake -S . -B build
cmake --build build
```

## Run

### Windows

```bash
build\\TodoList.exe
```

### Linux / macOS

```bash
./build/TodoList
```

## Test

```bash
ctest --test-dir build --output-on-failure
```

## Storage

Tasks are stored automatically in `data/tasks.db`. The file is generated at runtime and is intentionally ignored by Git.

## Application Menu

```text
1. Add task
2. List all tasks
3. View task
4. Edit task
5. Delete task
6. Mark task as completed
7. Mark task as pending
8. Search tasks
9. Filter tasks
0. Exit
```

## Concepts Demonstrated

This project demonstrates practical C++ concepts including classes, encapsulation, enums, vectors, algorithms, file I/O, exception handling, references, const-correctness, and separation of interface and implementation.

## License

This project is licensed under the MIT License. See `LICENSE` for details.
