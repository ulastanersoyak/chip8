add_library(display STATIC display.cpp)
target_include_directories(display PRIVATE ${CMAKE_SOURCE_DIR}/includes/display/ PRIVATE ${CMAKE_SOURCE_DIR}/includes/chip8)
