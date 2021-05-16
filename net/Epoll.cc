//
// Created by Sail on 2021/5/16.
//

#include "net/Epoll.h"
#include "net/Echo.h"

Epoll::Epoll(const Socket &listenSocket)
  : epfd_(epoll_create(10)),
    listenSocket_(listenSocket),
    revents_(16) {
  addToListen(listenSocket.getfd());
}

void Epoll::addToListen(int fd) {
  epoll_event evt{};
  evt.data.fd = fd;
  evt.events = EPOLLIN;
  epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &evt);
}

int Epoll::toSelect() {
  int numEvents = ::epoll_wait(epfd_, &*revents_.begin(), revents_.size(), -1);
  if (numEvents < 0) {
    std::cerr << "select error!!" << std::endl;
  } else {
    if (numEvents == revents_.size()) {
      revents_.resize(revents_.size() * 2);
    }
    handleActiveEvents(numEvents);
  }
  return 0;
}

void Epoll::handleActiveEvents(int numEvents) {
  std::cout << numEvents << std::endl;
  Echo echor;
  for (int i = 0; i < numEvents; ++i) {
    if (revents_[i].events & EPOLLIN) {
      if (revents_[i].data.fd == listenSocket_.getfd()) {
        int clientfd = listenSocket_.toAccept();
        addToListen(clientfd);
      } else {
        int ret = echor.echoOnClient(revents_[i].data.fd);
        if (ret == 0) {
          close(revents_[i].data.fd);
          epoll_ctl(epfd_, EPOLL_CTL_DEL, revents_[i].data.fd, nullptr);
        }
      }
    }
  }
}