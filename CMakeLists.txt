cmake_minimum_required(VERSION 3.22)

project(chip8 VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(
    -Wall -Wextra -Wpedantic -Werror -Wunused -Wuninitialized -Wshadow -Wconversion -Wsign-conversion 
    -Wfloat-equal -Weffc++ -Wold-style-cast -Wformat=2 -Wvla
  )
endif()

add_subdirectory(src)
