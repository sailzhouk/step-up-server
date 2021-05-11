//
// Created by Sail on 2021/5/10.
//

#ifndef STEP_UP_SERVER_SELECT_H
#define STEP_UP_SERVER_SELECT_H


#include <vector>
#include <sys/select.h>
#include <unistd.h>
#include "step1/Socket.h"

class Select {
public:
  Select(const Socket& listenSocket)
    : clients_(std::move(std::vector<int>(FD_SETSIZE, -1))),
      idx_(-1),
      maxfd_(listenSocket.getfd()),
      listenSocket_(listenSocket) {
    FD_ZERO(&allset_);
    FD_SET(listenSocket.getfd(), &allset_);
  }

  void addToListen(int fd);

  int toSelect();

  void handleActiveEvents(fd_set fdSet, int numEvents);

private:
  std::vector<int> clients_;
  fd_set allset_;
  int idx_;
  int maxfd_;
  Socket listenSocket_;
};


#endif //STEP_UP_SERVER_SELECT_H
