 # linux g++ Ninja环境下构建命令 cmake -S . -B build -G Ninja -D CMAKE_CXX_COMPILER=g++ 
cmake_minimum_required(VERSION 3.23)

project(QuantEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(preknown)
add_subdirectory(include)
add_subdirectory(tests)
