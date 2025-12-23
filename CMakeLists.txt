cmake_minimum_required(VERSION 3.16)
project(cli_log_analyzer VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_WERROR "Treat warnings as errors" OFF)

add_library(loglib
    src/parser.cpp
    src/stats.cpp
)
target_include_directories(loglib PUBLIC include)
target_compile_options(loglib PRIVATE -Wall -Wextra -Wpedantic)
if(ENABLE_WERROR)
  target_compile_options(loglib PRIVATE -Werror)
endif()

add_executable(log_analyzer src/main.cpp)
target_link_libraries(log_analyzer PRIVATE loglib)
target_compile_options(log_analyzer PRIVATE -Wall -Wextra -Wpedantic)

add_executable(unit_tests
    tests/test_stats.cpp
    tests/test_parser.cpp
)
target_link_libraries(unit_tests PRIVATE loglib)
target_compile_options(unit_tests PRIVATE -Wall -Wextra -Wpedantic)

enable_testing()
add_test(NAME unit_tests COMMAND unit_tests)
