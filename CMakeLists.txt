cmake_minimum_required(VERSION 3.21)
project(XRayMachine 
        LANGUAGES CXX
        VERSION 0.0.1)

# Modules List
# =================================
set(BUILD_Communication                   ON)
set(BUILD_ConfigurationFileParser         ON)
set(BUILD_Devices                         ON)
set(BUILD_Motors                          ON)
set(BUILD_PostProcessing                  ON)
set(BUILD_Scan                            ON)
set(BUILD_Sensors                         ON)
set(BUILD_Template                        OFF)
set(BUILD_UIManagementServer              ON)
set(BUILD_XRaySensor                      ON)
set(BUILD_Tests                           ON)
# =================================

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(UtilityFunctions)

if (WIN32)
    message(STATUS "Windows OS detected. Setting up WIN32 threads option")
    option(CMAKE_USE_WIN32_THREADS_INIT "using WIN32 threads" ON)
endif()

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
set(CMAKE_CXX_STANDARD 17)


set(XRAY_EXTERNAL_LIBRARIES_DIR "${PROJECT_SOURCE_DIR}/extern") #External Libraries location

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Project Version:  ${PROJECT_VERSION}")

# Setup External Repositories
# =================================
include(RepositoryFunctions)
include(FetchContent)

# Versions
set(GTEST_REPOSITORY_TAG "release-1.12.1") #old version release-1.11.0 
set(SPDLOG_REPOSITORY_TAG "v1.9.2")
set(sml_REPOSITORY_TAG "v1.1.5")

if(BUILD_Tests)
    include(CTest)
    include(GoogleTest)
endif()

setup_gtest_repository(${GTEST_REPOSITORY_TAG})
setup_spdlog_repository(${SPDLOG_REPOSITORY_TAG})

FetchContent_MakeAvailable(googletest spdlog)


# Setup Modules Directories
# =================================

add_subdirectory(extern/ini)
add_subdirectory(extern/json)
add_subdirectory(extern/sml)

add_subdirectory(modules)


