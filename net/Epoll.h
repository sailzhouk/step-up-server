//
// Created by Sail on 2021/5/16.
//

#ifndef STEP_UP_SERVER_EPOLL_H
#define STEP_UP_SERVER_EPOLL_H

#include <sys/epoll.h>
#include <net/Socket.h>
#include <vector>
class Epoll {
public:
  explicit Epoll(const Socket& listenSocket);
  void addToListen(int fd);
  int toSelect();
  void handleActiveEvents(int numEvents);
private:
  int epfd_;
  Socket listenSocket_;
  std::vector<epoll_event> revents_;
};


#endif //STEP_UP_SERVER_EPOLL_H
