cmake_minimum_required(VERSION 3.16)
project(step_up_server)


include_directories(${PROJECT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 20)

#add_library(step_up_server step1/Socket.h step1/ServerSelect.cc)
add_executable(step_up_server net/Socket.h ServerSelect.cc net/Select.cc net/Select.h net/Echo.h net/Sockaddr.h)