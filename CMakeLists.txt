cmake_minimum_required(VERSION 3.22)

if (NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif()

project(chip8 VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(
    -Wall -Wextra -Wpedantic -Werror -Wunused -Wuninitialized -Wshadow -Wconversion 
    -Wsign-conversion -Wfloat-equal -Weffc++ -Wold-style-cast -Wvexing-parse -Wformat=2 -Wvla -g
  )
endif()

add_subdirectory(src)
