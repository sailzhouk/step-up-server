//
// Created by Sail on 2021/5/10.
//
#include "net/Socket.h"
#include "net/EventLoop.h"
#include "net/Channel.h"
#include <unistd.h>
#include "net/Acceptor.h"
#include <cstdio>
#include <functional>

using namespace std::placeholders;
namespace step {
class Echo : Uncopyable {
 public:
  Echo(EventLoop* loop, Acceptor* acceptor)
      : acceptor_(acceptor),
        loop_(loop),
        channel_(acceptor->getChannel()),
        fd_(acceptor->getChannel()->getfd()) {
    acceptor->listen();
    acceptor->setConnectionCallback(std::bind(&Echo::echoOnClient, this, _1));
  }

  void echoOnClient(int fd) {
    auto ret = read(fd, buf_, sizeof(buf_));
    if (ret != 0) {
      write(fd, buf_, ret);
    }
  }

  void start() {
    loop_->loop();
  }
 private:
  int fd_;
  std::unique_ptr<Channel> channel_;
  EventLoop* loop_;
  std::unique_ptr<Acceptor> acceptor_;
  char buf_[BUFSIZ];
};
}
