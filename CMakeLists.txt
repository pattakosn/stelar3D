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

set(CMAKE_EXPORT_COMPILE_COMMANDS ON )

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

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

find_package(glad CONFIG REQUIRED)
find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)
find_package(assimp CONFIG REQUIRED)
#find_package(soil2 CONFIG REQUIRED)
list(APPEND LIBS SDL2::SDL2 SDL2::SDL2main assimp::assimp glad::glad )
list(APPEND LIBS ${CMAKE_DL_LIBS})

add_subdirectory(src)

message( "Using: ${LIBS}")
