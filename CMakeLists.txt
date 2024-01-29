cmake_minimum_required(VERSION 3.25)

project(platformer)

set(CMAKE_CXX_STANDARD 17 CACHE STRING "" FORCE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "" FORCE)

add_executable(main)
set_target_properties(main PROPERTIES OUTPUT_NAME ${CMAKE_PROJECT_NAME})
add_custom_command(TARGET main POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/res
  ${CMAKE_CURRENT_BINARY_DIR}/res)

add_subdirectory("src")
add_subdirectory("external")

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})
  set_property(TARGET main PROPERTY WIN32_EXECUTABLE true)
endif()
