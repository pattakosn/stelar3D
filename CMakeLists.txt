##NIKOS TEST set( CMAKE_TOOLCHAIN_FILE
##NIKOS TEST      ${CMAKE_CURRENT_SOURCE_DIR}/external/vcpkg/scripts/buildsystems/vcpkg.cmake
##NIKOS TEST      CACHE STRING "Vcpkg toolchain file"
##NIKOS TEST )
#if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
#  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
#      CACHE STRING "")
#endif()

cmake_minimum_required (VERSION 3.22.2 FATAL_ERROR)
project(stelar3D)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(NoInSourceBuilds)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_SHARED_LIBS OFF)

#list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/autovcpkg)
#include(AutoVcpkg)
#vcpkg_install(sdl2 glad glm assimp soil2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
SET(CMAKE_POLICY_DEFAULT_CMP0012 NEW)
SET(CMAKE_POLICY_DEFAULT_CMP0111 NEW)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

add_subdirectory(external/glm)
add_subdirectory(external/glad)
find_package(SDL2 REQUIRED)
find_package(assimp REQUIRED)
set_target_properties(assimp::assimp PROPERTIES MAP_IMPORTED_CONFIG_DEBUG Release)
# Workaround for flawed assimp package on Ubuntu: remove non-existing paths (like "/usr/lib/include")
if(UNIX AND assimp_FOUND AND ASSIMP_INCLUDE_DIRS)
    message(AUTHOR_WARNING "Calling ASSIMP workaround to remove non-existing paths")

    # Remove non-existing paths
    foreach(path ${ASSIMP_INCLUDE_DIRS})
        if(NOT EXISTS ${path})
            list(REMOVE_ITEM ASSIMP_INCLUDE_DIRS "${path}")
        endif()
    endforeach()

    # Check if ASSIMP_INCLUDE_DIRS contains at least one valid path
    set(VALID_ASSIMP_INCLUDE_DIRS False)
    foreach(path ${ASSIMP_INCLUDE_DIRS})
        if(EXISTS "${path}/assimp/ai_assert.h")
            set(VALID_ASSIMP_INCLUDE_DIRS True)
        endif()
    endforeach()

    # Find a valid include directory if necessary
    if(NOT ${VALID_ASSIMP_INCLUDE_DIRS})
        find_path(ASSIMP_FIXED_INCLUDE_DIR "assimp/ai_assert.h" PATHS "/usr/include")
        mark_as_advanced(ASSIMP_FIXED_INCLUDE_DIR)
        if(EXISTS ${ASSIMP_FIXED_INCLUDE_DIR})
            list(APPEND ASSIMP_INCLUDE_DIRS "${ASSIMP_FIXED_INCLUDE_DIR}")
        else()
            message(FATAL_ERROR "Could not find valid ASSIMP include directory, try to set ASSIMP_FIXED_INCLUDE_DIR to a valid path containing the ASSIMP header folder")
        endif()
    endif()

    message(STATUS "ASSIMP_INCLUDE_DIRS after workaround: " ${ASSIMP_INCLUDE_DIRS})
endif()
find_package(OpenMP)
find_package(OpenGL REQUIRED)
#add_definitions(${OPENGL_DEFINITIONS})
#find_package(X11 REQUIRED)
#set(LIBS ${LIBS} X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL dl pthread  stelar3D PARENT_SCOPE)
#example to handle shader/assets/libs maybe? add_custom_command(TARGET dear-imgui-conan
#example to handle shader/assets/libs maybe?     POST_BUILD
#example to handle shader/assets/libs maybe?     COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/assets/simple-shader.vs ${PROJECT_BINARY_DIR}
#example to handle shader/assets/libs maybe?     COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/assets/simple-shader.fs ${PROJECT_BINARY_DIR}
#example to handle shader/assets/libs maybe? )

add_library(imgui
        # Main Imgui files
        external/imgui/imgui.cpp
        external/imgui/imgui_draw.cpp
        external/imgui/imgui_tables.cpp
        external/imgui/imgui_widgets.cpp
        external/imgui/imgui_demo.cpp
        # SDL2+OpenGL-specific files
        external/imgui/backends/imgui_impl_sdl.cpp
        external/imgui/backends/imgui_impl_opengl3.cpp)
add_library(imgui::imgui ALIAS imgui)
target_link_libraries(imgui PUBLIC OpenGL::GL SDL2::SDL2)
target_include_directories(imgui PUBLIC external/imgui/ external/imgui/backends/)
target_compile_definitions(imgui PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)

include(CheckIPOSupported)
check_ipo_supported(RESULT IPOsupported OUTPUT IPOerror)
if(IPOsupported)
  message(STATUS "IPO / LTO enabled")
else()
  message(STATUS "IPO / LTO not supported: <${IPOerror}>")
endif()

add_executable(imgui-example external/imgui/examples/example_sdl_opengl3/main.cpp)
target_link_libraries(imgui-example imgui)
set_target_properties(imgui-example PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
if(IPOsupported)
    set_property(TARGET imgui-example PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

add_subdirectory(src)
