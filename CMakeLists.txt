cmake_minimum_required(VERSION 3.13)
project(Themulator)

set(CMAKE_C_STANDARD 99)

find_package(CURL REQUIRED)

file(GLOB SOURCES "src/*.c")
add_executable(thml ${SOURCES})

target_link_libraries(thml CURL::libcurl)

install(TARGETS thml DESTINATION bin)
