cmake_minimum_required(VERSION 4.3)
project(fusetoy C)

set(CMAKE_C_STANDARD 23)

find_package(PkgConfig REQUIRED)
pkg_check_modules(FUSE3 REQUIRED IMPORTED_TARGET fuse3)

add_executable(fusetoy main.c)

target_link_libraries(fusetoy PRIVATE PkgConfig::FUSE3)