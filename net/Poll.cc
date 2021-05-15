//
// Created by Sail on 2021/5/15.
//

#include "net/Poll.h"

#include "Echo.h"

Poll::Poll(const Socket &listenSocket)
  : clients_(std::move(std::vector<pollfd>(FD_SETSIZE))),
    maxIdx_(0),
    listenSocket_(listenSocket) {
  for (auto &client : clients_) {
    client.fd = -1;
  }
  clients_[0].fd = listenSocket.getfd();
  clients_[0].events = POLLIN;
}

void Poll::addToListen(int fd) {
  auto it = clients_.begin();
  bool flag = false;
  for (; it != clients_.end(); ++it) {
    if ((*it).fd == -1) {
      flag = true;
      (*it).events = POLLIN;
      (*it).fd = fd;
      break;
    }
  }
  if (!flag) {
    std::cerr << "too many clients!!" << std::endl;
  }
  maxIdx_ = std::max(int(it - clients_.begin()), maxIdx_);
}

int Poll::toSelect() {
  int numEvents = ::poll(&*clients_.begin(), maxIdx_+1, -1);
  if (numEvents < 0) {
    std::cerr << "select error!!" << std::endl;
  } else {
    handleActiveEvents(numEvents);
  }
  return 0;
}

void Poll::handleActiveEvents(int numEvents) {
  Echo echor;
  if (clients_[0].revents & POLLIN) {
    int clientfd = listenSocket_.toAccept();
    addToListen(clientfd);
    --numEvents;
  }
  for (int i = 1; i <= maxIdx_ && numEvents > 0; ++i) {
    if (clients_[i].fd < 0) {
      continue;
    }
    if (clients_[i].revents & POLLIN) {
      int ret = echor.echoOnClient(clients_[i].fd);
      if (ret == 0) {
        close(clients_[i].fd);
        clients_[i].fd = -1;
      }
      --numEvents;
    }
  }
}