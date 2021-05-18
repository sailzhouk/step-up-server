//
// Created by Sail on 2021/5/16.
//

#include "net/Channel.h"
#include "net/EventLoop.h"
#include <cassert>
#include <sys/epoll.h>
using namespace step;

const uint32_t Channel::kNoneEvent = 0;
const uint32_t Channel::kReadEvent = EPOLLIN | EPOLLPRI;
const uint32_t Channel::kWriteEvent = EPOLLOUT;

Channel::Channel(EventLoop* loop, int fd)
  : loop_(loop),
    fd_(fd),
    index_(kNew),
    events_(0),
    revents_(0),
    eventHandling_(false),
    addedToLoop_(false) {

}

Channel::~Channel() {
  assert(!eventHandling_);
  assert(!addedToLoop_);
}


void Channel::update() {
  addedToLoop_ = true;
  loop_->updateChannel(this);
}

void Channel::remove() {
  assert(isNoneEvent());
  addedToLoop_ = false;
  loop_->removeChannel(this);
}

void Channel::handleEvent() {
  eventHandling_ = true;

  if (revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)) {
    if (readCallback_) readCallback_(fd_);
  }
  if (revents_ & EPOLLOUT) {
    if (writeCallback_) writeCallback_();
  }
  eventHandling_ = false;
}