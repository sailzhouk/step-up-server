//
// Created by Sail on 2021/5/16.
//

#include "net/Epoll.h"
#include "net/Echo.h"
#include "net/Channel.h"
using namespace step;
const int kEVENTSSIZE = 16;
Epoll::Epoll()
  : epfd_(::epoll_create1(EPOLL_CLOEXEC)),
    revents_(kEVENTSSIZE) {
  if (epfd_ < 0) {
    abort();
  }
}

void Epoll::poll(ChannelList *activeChannels) {
//  activeChannels->clear();
  int numEvents = ::epoll_wait(epfd_, &*revents_.begin(), revents_.size(), -1);
  int savedErrno = errno;
  if (numEvents >= 0) {
    for (int i = 0; i < numEvents; ++i) {
      auto channel = static_cast<Channel *>(revents_[i].data.ptr);
      channel->setRevents(revents_[i].events);
      activeChannels->push_back(channel);
    }
    if (numEvents == revents_.size()) {
      revents_.resize(revents_.size() * 2);
    }
  } else {
    if (savedErrno != EINTR) {
      errno = savedErrno;
      std::cerr << "poll error" << std::endl;
      abort();
    }
  }
}

void Epoll::update(uint32_t op, Channel *channel) {
  epoll_event evt{};
  evt.data.fd = channel->getfd();
  evt.events = channel->getEvents();
  evt.data.ptr = channel;
  if (epoll_ctl(epfd_, op, channel->getfd(), &evt) < 0) {
    std::cerr << "epoll ctl error" << std::endl;
  }
}

void Epoll::updateChannel(Channel *channel) {
  const int index = channel->getIndex();
  int fd = channel->getfd();
  if (index == Channel::kNew || index == Channel::kDeleted) {
    // this channel is a new channel
    if (index == Channel::kNew) {
      channels_[fd] = channel;
    } else {
      // index == Channel::kDeleted
      assert(channels_[fd] == channel);
    }
    channel->setIndex(Channel::kAdded);
    update(EPOLL_CTL_ADD, channel);
  } else {
    // this channel is existed
    assert(index == Channel::kAdded);
    assert(channels_[fd] == channel);
    if (channel->isNoneEvent()) {
      // need to delete
      update(EPOLL_CTL_DEL, channel);
    } else {
      update(EPOLL_CTL_MOD, channel);
    }
  }
}

void Epoll::removeChannel(Channel *channel) {
  int fd = channel->getfd();
  int index = channel->getIndex();
  assert(channels_.find(fd) != channels_.end());
  assert(channels_[fd] == channel);
  assert(channel->isNoneEvent());
  channels_.erase(fd);
  if (index == Channel::kAdded) {
    update(EPOLL_CTL_DEL, channel);
  }
  channel->setIndex(Channel::kNew);
}

