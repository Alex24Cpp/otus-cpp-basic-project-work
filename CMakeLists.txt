cmake_minimum_required(VERSION 3.5)
project(reminder)
add_executable(reminder main.cpp reminder.cpp)
set_target_properties(reminder PROPERTIES CXX_STANDARD 23 CXX_STANDARD_REQUIRED ON)
target_compile_options(reminder PRIVATE
    -Wall
    -Wextra
    -pedantic
    -Werror
)