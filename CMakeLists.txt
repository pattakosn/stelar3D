set( CMAKE_TOOLCHAIN_FILE
     ${CMAKE_CURRENT_SOURCE_DIR}/external/vcpkg/scripts/buildsystems/vcpkg.cmake
     CACHE STRING "Vcpkg toolchain file"
)
#if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
#  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
#      CACHE STRING "")
#endif()
#include("cmake/HunterGate.cmake")
#HunterGate(
#    URL "https://github.com/cpp-pm/hunter/archive/v0.23.297.tar.gz"
#    SHA1 "3319fe6a3b08090df7df98dee75134d68e2ef5a3"
#)

cmake_minimum_required (VERSION 3.18)
project(stelar3D)
#list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
#list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
#CONAN_TEST include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
#CONAN_TEST conan_basic_setup()

include(noinsourcebuilds)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_SHARED_LIBS OFF)

#list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/autovcpkg)
#include(AutoVcpkg)
#vcpkg_install(sdl2 glad glm assimp soil2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

#hunter_add_package(SDL2)
#hunter_add_package(assimp)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

add_subdirectory(external/glm)
add_subdirectory(external/glad)
find_package(SDL2 CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(OpenMP)
#find_package(OpenGL REQUIRED)
#add_definitions(${OPENGL_DEFINITIONS})
#find_package(X11 REQUIRED)
#set(LIBS ${LIBS} X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL dl pthread  stelar3D PARENT_SCOPE)
#example to handle shader/assets/libs maybe? add_custom_command(TARGET dear-imgui-conan
#example to handle shader/assets/libs maybe?     POST_BUILD
#example to handle shader/assets/libs maybe?     COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/assets/simple-shader.vs ${PROJECT_BINARY_DIR}
#example to handle shader/assets/libs maybe?     COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/assets/simple-shader.fs ${PROJECT_BINARY_DIR}
#example to handle shader/assets/libs maybe? )

include(CheckIPOSupported)
check_ipo_supported(RESULT IPOsupported OUTPUT IPOerror)
if(IPOsupported)
  message(STATUS "IPO / LTO enabled")
else()
  message(STATUS "IPO / LTO not supported: <${IPOerror}>")
endif()

add_subdirectory(src)
