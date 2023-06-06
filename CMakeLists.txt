cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME "server")

project(${PROJECT_NAME})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -std=gnu99")

file(GLOB_RECURSE SOURCES src/*.c)

add_executable(${PROJECT_NAME} main.c ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC include)

if(WIN32)
  target_link_libraries(${PROJECT_NAME} wsock32 ws2_32)
endif()