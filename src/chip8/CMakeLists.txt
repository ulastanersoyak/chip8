add_library(chip8 STATIC chip8.cpp)
target_include_directories(chip8 PRIVATE ${CMAKE_SOURCE_DIR}/includes/chip8/)

