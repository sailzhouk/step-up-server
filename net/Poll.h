//
// Created by Sail on 2021/5/15.
//

#ifndef STEP_UP_SERVER_POLL_H
#define STEP_UP_SERVER_POLL_H
#include <sys/poll.h>
#include <vector>
#include <net/Socket.h>
class Poll {
public:
  explicit Poll(const Socket& listenSocket);

  void addToListen(int fd);

  int toSelect();

  void handleActiveEvents(int numEvents);

private:
  std::vector<pollfd> clients_;
  int maxIdx_;
  Socket listenSocket_;
};


#endif //STEP_UP_SERVER_POLL_H
