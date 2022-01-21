cmake_minimum_required(VERSION 3.22)

project(platformer VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_subdirectory(SDL)
include_directories(SDL/include)

add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
target_link_libraries(${PROJECT_NAME} SDL2main SDL2-static)