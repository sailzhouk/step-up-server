//
// Created by Sail on 2021/5/16.
//

#ifndef STEP_UP_SERVER_ACCEPTOR_H
#define STEP_UP_SERVER_ACCEPTOR_H

#include "utils/Uncopyable.h"
#include "net/Channel.h"
#include "net/Socket.h"
#include <memory>
#include <utility>

namespace step {
class EventLoop;

class Acceptor : Uncopyable {
 public:
  typedef std::function<void(int )> NewConnectionCallback;
  Acceptor(EventLoop *loop, const Sockaddr& sa, bool reusePort=true);
  void listen();
  // getter
  Channel* getChannel() { return &acceptChannel_; }
  // setter
  void setConnectionCallback(NewConnectionCallback cb) { callback_ = std::move(cb); }
  bool isListening() { return listening_; }

 private:
  typedef std::vector<std::shared_ptr<Channel>> ConnectionMap;
  void handleRead();
  EventLoop* loop_;
  Socket acceptSocket_;
  Channel acceptChannel_;
  bool listening_;
  NewConnectionCallback callback_;
  ConnectionMap connections_;
};
}
#endif //STEP_UP_SERVER_ACCEPTOR_H
