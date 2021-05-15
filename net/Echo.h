//
// Created by Sail on 2021/5/10.
//

#include <unistd.h>
#include <cstdio>
class Echo {
public:
  int echoOnClient(int fd) {
    auto ret = read(fd, buf_, sizeof(buf_));
    if (ret != 0) {
      write(fd, buf_, ret);
    }
    return ret;
  }
private:
  char buf_[BUFSIZ];
};