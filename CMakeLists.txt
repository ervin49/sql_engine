cmake_minimum_required(VERSION 3.14)
project(sql_engine)

# Folosim C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "src/*.cpp")

add_executable(sql_engine ${SOURCES})

target_include_directories(sql_engine PRIVATE ${PROJECT_SOURCE_DIR}/include)

set_target_properties(sql_engine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
