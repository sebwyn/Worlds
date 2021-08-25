cmake_minimum_required(VERSION 3.20)
project(Worlds)

set_property(GLOBAL PROPERTY ALLOW_DUPLICATE_CUSTOM_TARGETS  True)

include(FetchContent)

#TODO: change what files we build based on platform detection
file(GLOB src_files 
    src/Platform/Mac/*.cpp 

    src/Worlds/Core/*.cpp 
    src/Worlds/Utils/*.cpp
    src/Worlds/Bitmaps/*.cpp
    src/Worlds/Devices/*.cpp
    src/Worlds/Files/*.cpp

    src/Worlds/Graphics/*.cpp 
    src/Worlds/Graphics/Buffers/*.cpp 
    src/Worlds/Graphics/Commands/*.cpp 
    src/Worlds/Graphics/Descriptors/*.cpp 
    src/Worlds/Graphics/Images/*.cpp 
    src/Worlds/Graphics/Pipelines/*.cpp 
    src/Worlds/Graphics/Renderpass/*.cpp 
)

add_library(Worlds STATIC ${src_files})

set_property(TARGET Worlds PROPERTY CXX_STANDARD 17)
target_precompile_headers(Worlds PRIVATE src/wpch.hpp)

find_package(Vulkan REQUIRED)

find_package(volk 121 QUIET)
if(NOT volk_FOUND)
	set(_ACID_ALL_SYSTEM_LIBS false)
	FetchContent_Declare(volk
			URL https://github.com/zeux/volk/tarball/5d2e2cf91f8a1fe4156d4c6b5c814d45d733684a
			URL_MD5 8c9250d59f2fda4bb076c2bb2c05c06b
			)
	FetchContent_GetProperties(volk)
	if(NOT volk_POPULATED)
		FetchContent_Populate(volk)
		add_subdirectory(${volk_SOURCE_DIR} ${volk_BINARY_DIR})
	endif()
	set_target_properties(volk PROPERTIES FOLDER External)
	add_library(volk::volk ALIAS volk)
endif()

if(NOT glslang_FOUND)
	set(_ACID_ALL_SYSTEM_LIBS false)
	FetchContent_Declare(glslang
			URL https://github.com/KhronosGroup/glslang/archive/8.13.3559.tar.gz
			URL_MD5 cb32322377cee2bc1cee5b60ebe46133
			)
	FetchContent_GetProperties(glslang)
	if(NOT glslang_POPULATED)
		foreach(_glslang_option "BUILD_TESTING" "ENABLE_GLSLANG_BINARIES" "ENABLE_SPVREMAPPER" "ENABLE_HLSL" 
				"ENABLE_AMD_EXTENSIONS" "ENABLE_NV_EXTENSIONS"
				)
			set(${_glslang_option} OFF CACHE INTERNAL "")
		endforeach()
		foreach(_glslang_option "SKIP_GLSLANG_INSTALL" "ENABLE_OPT")
			set(${_glslang_option} ON CACHE INTERNAL "")
		endforeach()
		FetchContent_Populate(glslang)
		add_subdirectory(${glslang_SOURCE_DIR} ${glslang_BINARY_DIR})
	endif()
	set_target_properties(glslang PROPERTIES FOLDER External/glslang)
	set_target_properties(OGLCompiler PROPERTIES FOLDER External/glslang)
	set_target_properties(OSDependent PROPERTIES FOLDER External/glslang)
	set_target_properties(SPIRV PROPERTIES FOLDER External/glslang)

	# Used in target_link_libraries()
	set(GLSLANG_LIBRARIES SPIRV)
else()
	set(GLSLANG_INCLUDE_DIRS "${GLSLANG_INCLUDE_DIR}" "${SPIRV_INCLUDE_DIR}")
	set(GLSLANG_LIBRARIES glslang::glslang glslang::SPIRV)
endif()

add_subdirectory(ext/glfw)
add_subdirectory(ext/glm)
add_subdirectory(ext/spdlog)
add_subdirectory(ext/physfs)

target_include_directories(Worlds 
    PUBLIC 
    src/ 
    ext/ 
    ${Vulkan_INCLUDE_DIRS}
    ext/physfs/src
    $<$<BOOL:${GLSLANG_INCLUDE_DIRS}>:${GLSLANG_INCLUDE_DIRS}>
)
target_link_libraries(Worlds 
    glfw 
    ${GLFW_LIBRARIES} 
    glm
    spdlog
    ${Vulkan_LIBRARIES} 
    physfs
    ${GLSLANG_LIBRARIES}
    volk::volk
)

