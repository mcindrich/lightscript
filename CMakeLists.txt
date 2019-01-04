cmake_minimum_required(VERSION 3.9)

project(lightscript)

include_directories("./include")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB SOURCE_FILES "./src/lightscript/*.c")

add_executable(lightscript ${SOURCE_FILES} "./src/main.c")

if (UNIX)
    target_link_libraries(lightscript m)
endif (UNIX)