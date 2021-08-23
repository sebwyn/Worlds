cmake_minimum_required(VERSION 3.20)
project(Worlds)

include(ExternalProject)

#TODO: change what files we build based on platform detection
file(GLOB src_files src/Platform/Mac/*.cpp src/Platform/Vulkan/*.cpp src/Platform/Vulkan/Devices/*.cpp src/Platform/Vulkan/Renderpass/*.cpp src/Platform/Vulkan/Image/*.cpp src/Platform/Vulkan/Command/*.cpp src/Worlds/Core/*.cpp src/Worlds/Renderer/*.cpp)

add_library(Worlds STATIC ${src_files})

set_property(TARGET Worlds PROPERTY CXX_STANDARD 17)

target_precompile_headers(Worlds PRIVATE src/wpch.hpp)

find_package(Vulkan REQUIRED)

set_property(GLOBAL PROPERTY ALLOW_DUPLICATE_CUSTOM_TARGETS  True)

add_subdirectory(ext/glfw)
add_subdirectory(ext/glm)
add_subdirectory(ext/spdlog)
add_subdirectory(ext/physfs)
target_include_directories(Worlds PUBLIC src/ ext/ ${Vulkan_INCLUDE_DIRS})
target_link_libraries(Worlds glfw ${GLFW_LIBRARIES} ${Vulkan_LIBRARIES} glm spdlog)

