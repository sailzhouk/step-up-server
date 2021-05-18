//
// Created by Sail on 2021/5/17.
//

#ifndef STEP_UP_SERVER_EVENTLOOP_H
#define STEP_UP_SERVER_EVENTLOOP_H

#include <atomic>
#include <deque>
#include <functional>
#include <memory>

#include "utils/Uncopyable.h"

namespace step {
class Epoll;
class Channel;
class EventLoop : Uncopyable {
 public:
  typedef std::function<void()> Functor;

  EventLoop();
  ~EventLoop() = default;  // force out-line dtor, for std::unique_ptr members.

  void loop();
  void quit();
  void runInLoop(Functor cb);

  void updateChannel(Channel* channel);
  void removeChannel(Channel* channel);
  bool hasChannel(Channel* channel);

 private:
//  typedef std::vector<Channel*> ChannelList;
  // change point:
  using ChannelList = std::deque<Channel*>;

  bool eventHandling_;
  std::atomic<bool> looping_;
  std::atomic<bool> quit_;
  std::unique_ptr<Epoll> epoller_;
//  std::unique_ptr<Channel> wakeupChannel_;
  ChannelList activeChannels_;
  Channel* currentActiveChannel_;
};
}


#endif  // STEP_UP_SERVER_EVENTLOOP_H
