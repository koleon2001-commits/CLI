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

# ---- Tests (split executables; each file has its own main) ----
add_executable(test_parser tests/test_parser.cpp)
target_link_libraries(test_parser PRIVATE loglib)
target_compile_options(test_parser PRIVATE -Wall -Wextra -Wpedantic)

add_executable(test_stats tests/test_stats.cpp)
target_link_libraries(test_stats PRIVATE loglib)
target_compile_options(test_stats PRIVATE -Wall -Wextra -Wpedantic)

enable_testing()
add_test(NAME test_parser COMMAND test_parser)
add_test(NAME test_stats COMMAND test_stats)
