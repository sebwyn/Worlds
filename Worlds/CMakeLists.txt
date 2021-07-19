cmake_minimum_required(VERSION 3.20)

project(Worlds)

file(GLOB src_files src/Worlds/Core/*.cpp)

add_library(Worlds STATIC ${src_files})

set_property(TARGET Worlds PROPERTY CXX_STANDARD 17)

target_precompile_headers(Worlds PRIVATE src/wpch.hpp)

find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})

add_subdirectory(ext/glfw)
add_subdirectory(ext/glm)
target_include_directories(Worlds PUBLIC src/ ext/)
target_link_libraries(Worlds glfw ${GLFW_LIBRARIES} ${Vulkan_LIBRARIES} glm)

