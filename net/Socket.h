//
// Created by Sail on 2021/5/9.
//

#ifndef STEP_UP_SERVER_SOCKET_H
#define STEP_UP_SERVER_SOCKET_H

#include "net/Sockaddr.h"
#include "utils/Uncopyable.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <cassert>
#include <unistd.h>
#include <iostream>

namespace step {
class Socket : Uncopyable {
 public:
  Socket()
      : fd_(::socket(AF_INET, SOCK_STREAM, 0)),
        sa_(nullptr) {
  }

  ~Socket() {
    close(fd_);
  }

  void toBind(Sockaddr& sa) {
    assert(::bind(fd_, sa.getSockaddr(), sa.getlen()) == 0);
    sa_ = &sa;
  }

  void toListen(int backlog = 5) const {
    if(::listen(fd_, backlog) != 0) {
      std::cerr << "listen failed: " << errno << std::endl;
      abort();
    }
  }

  int toAccept() {
    assert(sa_ != nullptr);
    socklen_t socklen = sa_->getlen();
    int retfd;
    while (true) {
      retfd = ::accept(fd_, sa_->getSockaddr(), &socklen);
      if (retfd == -1) {
        if (errno == EINTR) {
          continue;
        } else {
          std::cerr << "accept error !!" << std::endl;
          abort();
        }
      } else {
        break;
      }
    }
    return retfd;
  }

  void toConnect(Sockaddr& saTarget) const {
    assert(::connect(fd_, saTarget.getSockaddr(), saTarget.getlen()) == 0);
  };

  void setReuseAddr(bool on) const {
    int optval = on ? 1 : 0;
    ::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR,
                 &optval, static_cast<socklen_t>(sizeof optval));
  }

  void setReusePort(bool on) const {
    int optval = on ? 1 : 0;
    ::setsockopt(fd_, SOL_SOCKET, SO_REUSEPORT,
                 &optval, static_cast<socklen_t>(sizeof optval));
  }

  // getter
  int getfd() const { return fd_; }

 private:
  Sockaddr* sa_{};
  int fd_;
};
}
#endif //STEP_UP_SERVER_SOCKET_H