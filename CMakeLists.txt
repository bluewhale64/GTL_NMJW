cmake_minimum_required(VERSION 3.20)
project("Game Testing Lab")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
#a nice bit of wishful thinking
option("GTL_NMJW_CONSOLE" "Use vendored libraries" OFF)

if("GTL_NMJW_CONSOLE")
    add_subdirectory(vendored/sdl EXCLUDE_FROM_ALL)
else()
    # 1. Look for a SDL2 package, 2. look for the SDL2 component and 3. fail if none can be found
    find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)

    # 1. Look for a SDL2 package, 2. Look for the SDL2maincomponent and 3. DO NOT fail when SDL2main is not available
    find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
endif()

add_executable("GTL_NMJW" "main.cpp")
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

if(TARGET SDL2::SDL2main)
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
    target_link_libraries("GTL_NMJW" PRIVATE SDL2::SDL2main)
endif()
target_link_libraries("GTL_NMJW" PRIVATE glfw GLEW::GLEW OpenGL::GL SDL2::SDL2)