cmake_minimum_required(VERSION 3.16)
project(step_up_server)


include_directories(${PROJECT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 20)

#add_library(step_up_server step1/Socket.h step1/ServerSelect.cc)
add_executable(step_up_server step1/Socket.h step1/ServerSelect.cc step1/Select.cc step1/Select.h step1/Echo.h step1/Sockaddr.h)