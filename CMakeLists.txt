cmake_minimum_required(VERSION 2.6)
project(prepend)

add_executable(prepend main.cpp)

install(TARGETS prepend RUNTIME DESTINATION bin)
