cmake_minimum_required(VERSION 3.20)
project("Game Testing Lab")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
#a nice bit of wishful thinking
option("GTL_NMJW_CONSOLE" "Use vendored libraries" OFF)

if("GTL_NMJW_CONSOLE")
    add_subdirectory(vendored/sdl EXCLUDE_FROM_ALL)
else()
    # 1. Look for a SDL3 package, 2. look for the SDL3 component and 3. fail if none can be found
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3)

    # 1. Look for a SDL3 package, 2. Look for the SDL3maincomponent and 3. DO NOT fail when SDL3main is not available
    find_package(SDL3 REQUIRED CONFIG COMPONENTS SDL3main)
endif()

add_executable("GTL_NMJW" "main.cpp")
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL3 REQUIRED)

if(TARGET SDL3::SDL3main)
    # It has an implicit dependency on SDL3 functions, so it MUST be added before SDL3::SDL3 (or SDL3::SDL3-static)
    target_link_libraries("GTL_NMJW" PRIVATE SDL3::SDL3main)
endif()
target_link_libraries("GTL_NMJW" PRIVATE GLEW::GLEW OpenGL::GL SDL3::SDL3)