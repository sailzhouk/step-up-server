//
// Created by Sail on 2021/5/16.
//

#include "net/Acceptor.h"
#include "net/EventLoop.h"
#include "net/Sockaddr.h"
using namespace step;


Acceptor::Acceptor(EventLoop *loop, const Sockaddr& sa, bool reusePort)
  : loop_(loop),
    acceptSocket_(Socket()),
    acceptChannel_(loop_, acceptSocket_.getfd()),
    listening_(false) {
  acceptSocket_.setReuseAddr(true);
  acceptSocket_.setReusePort(reusePort);
  acceptSocket_.toBind(const_cast<Sockaddr&>(sa));
  acceptChannel_.setReadCallback(std::bind(&Acceptor::handleRead, this));
}

void Acceptor::listen() {
  acceptSocket_.toListen();
  acceptChannel_.enableRead();
}

void Acceptor::handleRead() {
  int connfd = acceptSocket_.toAccept();
  if (connfd >= 0) {
    std::shared_ptr<Channel> newchannel(new Channel(loop_, connfd));
    newchannel->setReadCallback(callback_);
    newchannel->enableRead();
    connections_.push_back(newchannel);
  }
}