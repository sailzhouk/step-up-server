//
// Created by Sail on 2021/5/17.
//

#include "net/EventLoop.h"
#include "net/Channel.h"
#include "net/Epoll.h"

using namespace step;

EventLoop::EventLoop()
  : looping_(false),
    eventHandling_(false),
    quit_(false),
    epoller_(new Epoll()),
    currentActiveChannel_(nullptr) {

}

void EventLoop::loop() {
  assert(!looping_);
  looping_ = true;
  quit_ = false;
  while (!quit_) {
    epoller_->poll(&activeChannels_);
    eventHandling_ = true;
    // Handle each active channel
    while (!activeChannels_.empty()) {
      auto channel = activeChannels_.front();
      activeChannels_.pop_front();
      currentActiveChannel_ = channel;
      currentActiveChannel_->handleEvent();
    }
    currentActiveChannel_ = nullptr;
    eventHandling_ = false;
  }
  looping_ = false;
}

void EventLoop::quit() {
  quit_ = true;
}

void EventLoop::runInLoop(Functor cb) { cb(); }

void EventLoop::updateChannel(Channel *channel) {
  epoller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel *channel) {
  if (eventHandling_) {
    assert(currentActiveChannel_ != channel ||
           std::find(activeChannels_.begin(), activeChannels_.end(), channel) ==
               activeChannels_.end());
  }
  epoller_->removeChannel(channel);
}