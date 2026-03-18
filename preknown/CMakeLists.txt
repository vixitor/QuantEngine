cmake_minimum_required(VERSION 3.16)

project(preknown LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB PREKNOWN_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

foreach(src IN LISTS PREKNOWN_SOURCES)
    get_filename_component(target "${src}" NAME_WE)
    add_executable(${target} "${src}")
endforeach()
