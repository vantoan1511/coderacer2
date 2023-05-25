cmake_minimum_required(VERSION 3.1)

project(CodeRace VERSION 1.0 LANGUAGES C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

set (CMAKE_C_STANDARD 99)
set (CMAKE_C_FLAGS "-Werror -Wall -Wextra -pedantic")

add_executable(application main.c code_with_bugs.c crc32.c)

