cmake_minimum_required(VERSION 3.10)
project(sql_engine)

set(CMAKE_CXX_STANDARD 14)

add_executable(sql_engine 
    main.cpp
    Application.h
    Table.h
)