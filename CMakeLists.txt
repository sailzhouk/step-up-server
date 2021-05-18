cmake_minimum_required(VERSION 3.16)
project(step_up_server)


include_directories(${PROJECT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 20)

#add_library(step_up_server step1/Socket.h step1/ServerSelect.cc)
add_executable(step_up_server net/Socket.h test.cc net/Echo.h net/Sockaddr.h net/Epoll.cc net/Epoll.h net/Acceptor.cc net/Acceptor.h net/Channel.cc net/Channel.h utils/Uncopyable.h net/EventLoop.cc net/EventLoop.h net/Socket.cc)