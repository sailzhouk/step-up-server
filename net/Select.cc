//
// Created by Sail on 2021/5/10.
//

#include "step1/Select.h"

#include <iostream>
#include "Echo.h"

void Select::addToListen(int fd) {
  int i = 0;
  bool flag = false;
  for (; i < clients_.size(); ++i) {
    if (clients_[i] == -1) {
      flag = true;
      clients_[i] = fd;
      break;
    }
  }
  if (!flag) {
    std::cerr << "too many clients!!" << std::endl;
  }
  FD_SET(fd, &allset_);
  maxfd_ = std::max(maxfd_, fd);
  idx_ = std::max(idx_, i);
}

int Select::toSelect() {
  fd_set rset = allset_;
  int numEvents = ::select(maxfd_ + 1, &rset, nullptr, nullptr, nullptr);
  if (numEvents < 0) {
    std::cerr << "select error!!" << std::endl;
  } else {
    handleActiveEvents(rset, numEvents);
  }
  return 0;
}

void Select::handleActiveEvents(fd_set fdSet, int numEvents) {
  Echo echor;
  // for connection
  if (FD_ISSET(listenSocket_.getfd(), &fdSet)) {
    int clientfd = listenSocket_.toAccept();
    addToListen(clientfd);
    --numEvents;
  }
  for (int i = 0; i <= idx_ && numEvents > 0; ++i) {
    if (clients_[i] < 0) {
      continue;
    }
    if (FD_ISSET(clients_[i], &fdSet)) {
      int ret = echor.echoOnClient(clients_[i]);
      if (ret == 0) {
        close(clients_[i]);
        FD_CLR(clients_[i], &allset_);
        clients_[i] = -1;
      }
      --numEvents;
    }
  }
}