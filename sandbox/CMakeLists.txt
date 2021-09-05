cmake_minimum_required(VERSION 3.20)
project(Sandbox)

file(GLOB src_files src/*.cpp) 

add_executable(Sandbox ${src_files})

set_property(TARGET Sandbox PROPERTY CXX_STANDARD 17)

get_target_property(Worlds_Includes Worlds INCLUDE_DIRECTORIES)
message(${Worlds_Includes})

target_link_libraries(Sandbox Worlds)
target_include_directories(Sandbox PUBLIC src )
