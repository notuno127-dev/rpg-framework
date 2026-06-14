cmake_minimum_required(VERSION 3.25)

project(rpg-framework LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL3 CONFIG REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/Application.cpp
    src/Application.h
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        SDL3::SDL3
)
