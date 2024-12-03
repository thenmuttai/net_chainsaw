# Specify the executable
add_executable(time_server main.c)

# Link the helper library to main.c
target_link_libraries(time_server PRIVATE helper)

# Link the winsock library
target_link_libraries(time_server PRIVATE ws2_32)

# Specify the include directory for main.c
target_include_directories(time_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../include)