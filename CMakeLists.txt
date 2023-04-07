cmake_minimum_required(VERSION 3.20.0)
project(firstgame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED_ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(THIRD_PARTY_DIRECTORY ${CMAKE_SOURCE_DIR}/third_party)
set(PROJECT_DIRECTORY ${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME})

include_directories(.)
include_directories(${THIRD_PARTY_DIRECTORY})

add_subdirectory(firstgame/main)
add_subdirectory(firstgame/stackoverflow)
