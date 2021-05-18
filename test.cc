//
// Created by Sail on 2021/5/10.
//

#include "net/Socket.h"
#include "net/Sockaddr.h"
#include "net/Epoll.h"
#include "net/EventLoop.h"
#include "net/Channel.h"
#include "net/Echo.h"
#include "net/Acceptor.h"

using namespace step;

int main(int argc, char *argv[]) {
  const std::string ipAddress = "127.0.0.1";
  const int port = 6666;
  Sockaddr sa = Sockaddr(ipAddress, port);

//  Socket listenSocket;
//  listenSocket.toBind(sa);
  EventLoop loop;
  Acceptor acceptor(&loop, sa);
  Echo echor(&loop, &acceptor);
  echor.start();
}