//
// Created by Sail on 2021/5/16.
//

#ifndef STEP_UP_SERVER_CHANNEL_H
#define STEP_UP_SERVER_CHANNEL_H
#include <cstdint>
#include <functional>
#include "utils/Uncopyable.h"

namespace step {
class EventLoop;
class Channel : Uncopyable {
 public:
  typedef std::function<void()> EventCallback;
  typedef std::function<void(int)> ReadCallback;
  Channel(EventLoop* loop, int fd);

  ~Channel();

  void enableRead() { events_ |= kReadEvent; update(); }
  void disableRead() { events_ &= ~kReadEvent; update(); }
  void enableWrite() { events_ |= kWriteEvent; update(); }
  void disableWrite() { events_ &= ~kWriteEvent; update(); }
  void disableAll() { events_ &= kNoneEvent; update(); }

  bool isNoneEvent() const { return events_ == kNoneEvent; }
  bool isReading() const { return events_ & kReadEvent; }
  bool isWriting() const { return events_ & kWriteEvent; }

  void update();
  void remove();
  void handleEvent();
  // getters
  int getfd() const { return fd_; }
  uint32_t getEvents() const { return events_; }
  int getIndex() const { return index_; }

  //setters
  void setIndex(int idx) { index_ = idx; }
  void setRevents(uint32_t evt) { revents_ = evt; }
  void setReadCallback(ReadCallback cb) { readCallback_ = std::move(cb); }
  void setWriteCallback(EventCallback cb) { writeCallback_ = std::move(cb); }
  void setCloseCallback(EventCallback cb) { closeCallback_ = std::move(cb); }
  void setErrorCallback(EventCallback cb) { errorCallback_ = std::move(cb);  }

  static const int kNew = -1;
  static const int kAdded = 1;
  static const int kDeleted = 2;

 private:
  // 用于设置events的值
  static const uint32_t kNoneEvent;
  static const uint32_t kReadEvent;
  static const uint32_t kWriteEvent;

  EventLoop* loop_;
  const int fd_;
  uint32_t events_;
  uint32_t revents_;
  bool eventHandling_;
  bool addedToLoop_;
  int index_;
  ReadCallback readCallback_;
  EventCallback writeCallback_;
  EventCallback closeCallback_;
  EventCallback errorCallback_;
};

}


#endif //STEP_UP_SERVER_CHANNEL_H
