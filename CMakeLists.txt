#cmake_minimum_required(VERSION 3.1)

#project(CodeRace VERSION 1.0 LANGUAGES C)
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

#set (CMAKE_C_STANDARD 99)
#add_executable(application track2/main.c track2/code_with_bugs.c track2/crc32.c)


cmake_minimum_required(VERSION 3.1)

project(CodeRace VERSION 1.0 LANGUAGES C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

set(CMAKE_C_STANDARD 99)

# Tìm tất cả các tệp nguồn trong thư mục "track1" và "track2"
file(GLOB TRACK1_SOURCES "track1/*.c")
file(GLOB TRACK2_SOURCES "track2/*.c")

add_executable(application ${TRACK1_SOURCES} ${TRACK2_SOURCES})
