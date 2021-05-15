//
// Created by Sail on 2021/5/10.
//

#include "net/Socket.h"
#include "net/Select.h"
#include "net/Sockaddr.h"
#include "net/Poll.h"
int main(int argc, char *argv[]) {
  const std::string ipAddress = "127.0.0.1";
  const int port = 6666;
  Sockaddr sa = Sockaddr(ipAddress, port);

  Socket listenSocket;
  listenSocket.toBind(sa);
  listenSocket.toListen();
//  int fdc = listenSocket.toAccept();
//  std::cout << "accept ok" << fdc << std::endl;
  Select selector(listenSocket);
  Poll poller(listenSocket);
  while (true) {
//    selector.toSelect();
    poller.toSelect();
  }
//  close(fdc);
  return 0;
}