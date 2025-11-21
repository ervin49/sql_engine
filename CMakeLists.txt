cmake_minimum_required(VERSION 3.14)
project(sql_engine)

set(CMAKE_CXX_STANDARD 14)

# === GoogleTest ===
include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# === Executabil principal ===
add_executable(sql_engine
        main.cpp
        Application.h
        Table.h
        Index_Catalog.h
        Index.h
        globals.h
        globals.cpp
)

# === Teste ===
add_executable(sql_engine_tests
        test/Table_test.cpp
        test/Catalog_test.cpp
        test/Application_test.cpp
        Index_Catalog.h
        Index.h
        globals.h
        globals.cpp
)

target_link_libraries(sql_engine_tests
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(sql_engine_tests)

