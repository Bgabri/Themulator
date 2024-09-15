cmake_minimum_required(VERSION 3.10)
project(Themulator)

set(CMAKE_C_STANDARD 99)

file(GLOB SOURCES "src/*.c")
add_executable(thml ${SOURCES})
install(TARGETS thml DESTINATION bin)
