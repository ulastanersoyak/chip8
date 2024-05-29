add_executable(test test.cpp)
target_include_directories(test PRIVATE ${CMAKE_SOURCE_DIR}/includes/chip8/ ${CMAKE_SOURCE_DIR}/includes/display/)
target_link_libraries(test PUBLIC chip8 display SDL2main SDL2)
