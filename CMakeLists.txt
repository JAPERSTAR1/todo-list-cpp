cmake_minimum_required(VERSION 3.16)
project(TodoList VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(TodoList
    src/main.cpp
    src/Task.cpp
    src/TodoList.cpp
)

target_include_directories(TodoList PRIVATE include)

if (MSVC)
    target_compile_options(TodoList PRIVATE /W4)
else()
    target_compile_options(TodoList PRIVATE -Wall -Wextra -Wpedantic)
endif()

enable_testing()
add_executable(TodoListTests
    tests/TodoListTests.cpp
    src/Task.cpp
    src/TodoList.cpp
)
target_include_directories(TodoListTests PRIVATE include)
add_test(NAME TodoListTests COMMAND TodoListTests)
