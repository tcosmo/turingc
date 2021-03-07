cmake_minimum_required(VERSION 2.8.12)
project(turingc)

add_definitions("-std=c11")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

file(GLOB SOURCES "src/*.c")
add_executable(turingc ${SOURCES})
target_link_libraries(turingc ${CONAN_LIBS})

set (source "${CMAKE_SOURCE_DIR}/assets")
set (destination "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets")
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${destination} 
    DEPENDS ${destination}
    COMMENT "symbolic link resources folder from ${source} => ${destination}"
)