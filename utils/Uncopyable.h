//
// Created by Sail on 2021/5/16.
//

#ifndef STEP_UP_SERVER_UNCOPYABLE_H
#define STEP_UP_SERVER_UNCOPYABLE_H

namespace step{
class Uncopyable {
 public:
  Uncopyable(const Uncopyable &) = delete;
  void operator=(const Uncopyable &) = delete;

 protected:
  Uncopyable() = default;
  ~Uncopyable() = default;
};
}
#endif  // STEP_UP_SERVER_UNCOPYABLE_H