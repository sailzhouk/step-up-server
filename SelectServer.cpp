//
// Created by Sail on 2021/5/9.
//

#include <sys/socket.h>
#include <arpa/inet.h>

/// server with select
int main(int argc, char *argv[]) {
  // create socket
  auto fds = socket(AF_INET, SOCK_STREAM, 0);

  return 0;
}