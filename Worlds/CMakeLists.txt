cmake_minimum_required(VERSION 3.20)
project(Worlds)

set_property(GLOBAL PROPERTY ALLOW_DUPLICATE_CUSTOM_TARGETS  True)
set(BUILD_SHARED_LIBS False)

include(FetchContent)

#TODO: change what files we build based on platform detection
file(GLOB src_files 
    src/Platform/Mac/*.cpp 

    src/Worlds/Core/*.cpp 
    src/Worlds/Utils/*.cpp
    src/Worlds/Bitmaps/*.cpp
    src/Worlds/Devices/*.cpp
    src/Worlds/Files/*.cpp
    src/Worlds/Models/*.cpp
    src/Worlds/Models/Shapes/*.cpp
    src/Worlds/Scenes/*.cpp

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

find_package(glfw3 3.3 QUIET)
if(NOT TARGET glfw)
	set(_ACID_ALL_SYSTEM_LIBS false)
	FetchContent_Declare(glfw3
			URL https://github.com/glfw/glfw/archive/3.3.2.tar.gz
			URL_MD5 865e54ff0a100e9041a40429db98be0b
			)
	FetchContent_GetProperties(glfw3)
	if(NOT glfw3_POPULATED)
		foreach(_glfw3_option "GLFW_BUILD_TESTS" "GLFW_BUILD_EXAMPLES" "GLFW_BUILD_DOCS" "GLFW_INSTALL")
			set(${_glfw3_option} OFF CACHE INTERNAL "")
		endforeach()
		FetchContent_Populate(glfw3)
		add_subdirectory(${glfw3_SOURCE_DIR} ${glfw3_BINARY_DIR})
	endif()
	set_target_properties(glfw PROPERTIES FOLDER External)
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

find_package(PhysFS 3.0.1 QUIET)
if(NOT PhysFS_FOUND)
	set(_ACID_ALL_SYSTEM_LIBS false)
	FetchContent_Declare(physfs
			URL https://github.com/EQMG/physfs/tarball/e5da0dbd4674fa421ace0efbf0ad0dd692995b08
			URL_MD5 d3bb92335e9565c4101ee3ea10b42e43
			)
	FetchContent_GetProperties(physfs)
	if(NOT physfs_POPULATED)
		set(PHYSFS_BUILD_STATIC NOT${BUILD_SHARED_LIBS} CACHE INTERNAL "")
		set(PHYSFS_BUILD_SHARED ${BUILD_SHARED_LIBS} CACHE INTERNAL "")
		set(PHYSFS_BUILD_TEST OFF CACHE INTERNAL "")
		set(PHYSFS_TARGETNAME_DOCS "physfs-docs" CACHE INTERNAL STRING)
		set(PHYSFS_TARGETNAME_DIST "physfs-dist" CACHE INTERNAL STRING)
		set(PHYSFS_TARGETNAME_UNINSTALL "physfs-uninstall" CACHE INTERNAL STRING)
		set(CMAKE_DISABLE_FIND_PACKAGE_Doxygen TRUE)
		FetchContent_Populate(physfs)
		add_subdirectory(${physfs_SOURCE_DIR} ${physfs_BINARY_DIR})
	endif()
	set_target_properties(physfs-static PROPERTIES FOLDER External/physfs)
	if(TARGET physfs)
		set_target_properties(physfs PROPERTIES FOLDER External/physfs)
	endif()
	if(TARGET physfs-docs)
		set_target_properties(physfs-docs PROPERTIES FOLDER External/physfs)
	endif()
	if(TARGET physfs-uninstall)
		set_target_properties(physfs-uninstall PROPERTIES FOLDER External/physfs)
	endif()

	# PhysFS disables rpath on GNU, when disabled Acid will not run on Unix.
	if(CMAKE_COMPILER_IS_GNUCC)
		set(CMAKE_SKIP_RPATH OFF CACHE BOOL "Skip RPATH" FORCE)
	endif()

	set(PHYSFS_INCLUDE_DIR "${physfs_SOURCE_DIR}/src")
	# Set here so we can use it in Sources/CMakeLists.txt
	if(PHYSFS_BUILD_STATIC)
		set(PHYSFS_LIBRARY physfs-static)
	else()
		set(PHYSFS_LIBRARY physfs)
	endif()
endif()

if(APPLE)
    set(OTHER_LDFLAGS ${OTHER_LDFLAGS} "-framework IOKit -framework Foundation -framework CoreFoundation")
endif()

add_subdirectory(ext/glm)
add_subdirectory(ext/spdlog)
add_subdirectory(ext/entt)

target_include_directories(Worlds 
    PUBLIC 
    src/ 
    ext/ 
    ${Vulkan_INCLUDE_DIRS}
    $<$<BOOL:${GLSLANG_INCLUDE_DIRS}>:${GLSLANG_INCLUDE_DIRS}>
    $<$<BOOL:${PHYSFS_INCLUDE_DIR}>:${PHYSFS_INCLUDE_DIR}>
)
target_link_libraries(Worlds 
  PUBLIC
    glm
    spdlog
    ${Vulkan_LIBRARIES} 
    ${OTHER_LDFLAGS}
    EnTT::EnTT

    volk::volk
    glfw
    ${GLSLANG_LIBRARIES}
    ${PHYSFS_LIBRARY}
)

