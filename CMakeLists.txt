cmake_minimum_required(VERSION 3.8)
project(filesystem)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp tree.h server.h client.h constants.h)
add_executable(inda_encryption ${SOURCE_FILES})