cmake_minimum_required(VERSION 3.14)
project(sql_engine)

# Folosim C++11
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(sql_engine
        main.cpp
        globals.cpp
        StatusManager.h
        first_configuration.h
        Menu.h)
target_include_directories(sql_engine PRIVATE ${PROJECT_SOURCE_DIR})

# Setăm output în bin/
set_target_properties(sql_engine PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
)
