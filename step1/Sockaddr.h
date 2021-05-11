//
// Created by Sail on 2021/5/11.
//

#ifndef STEP_UP_SERVER_SOCKADDR_H
#define STEP_UP_SERVER_SOCKADDR_H


#include "arpa/inet.h"
#include <string>
#include <cassert>

class Sockaddr {
public:
  Sockaddr() = default;
  Sockaddr(const std::string& ipAddr, uint16_t port) {
    sockaddrIn_.sin_family = AF_INET;
    sockaddrIn_.sin_port = htons(port);
    assert(inet_pton(AF_INET, ipAddr.c_str(), static_cast<void*>(&sockaddrIn_.sin_addr.s_addr)) == 1);
  }

  Sockaddr(const uint32_t ipAddr, uint16_t port) {
    sockaddrIn_.sin_family = AF_INET;
    sockaddrIn_.sin_port = htons(port);
    sockaddrIn_.sin_addr.s_addr = htonl(ipAddr);
  }

  sockaddr *getSockaddr() {
    return reinterpret_cast<sockaddr *>(&sockaddrIn_);
  }

  sockaddr_in *getSockaddrIn() {
    return &sockaddrIn_;
  }

  socklen_t getlen() const {
    return sizeof(sockaddrIn_);
  }
private:
  sockaddr_in sockaddrIn_{};
};
#endif //STEP_UP_SERVER_SOCKADDR_H
