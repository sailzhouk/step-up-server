//
// Created by Sail on 2021/5/16.
//

#ifndef STEP_UP_SERVER_EPOLL_H
#define STEP_UP_SERVER_EPOLL_H

#include "net/Socket.h"
#include <sys/epoll.h>

#include <deque>
#include <unordered_map>
#include <vector>

namespace step {
class Channel;
class Epoll : Uncopyable {
 public:
//  typedef std::vector<Channel*> ChannelList;
  using ChannelList = std::deque<Channel*>;
  Epoll();
  ~Epoll() { ::close(epfd_); }
  void poll(ChannelList *activeChannels);
  void updateChannel(Channel* channel);
  void removeChannel(Channel* channel);

 private:
  void update(uint32_t op, Channel* channel);

  int epfd_;
  std::vector<epoll_event> revents_;
  std::unordered_map<int, Channel*> channels_;
};
}


#endif //STEP_UP_SERVER_EPOLL_H
