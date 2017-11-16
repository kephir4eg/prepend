cmake_minimum_required(VERSION 3.0)
project(prepend)

add_executable(prepend prepend.cpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

install(TARGETS prepend RUNTIME DESTINATION bin)
