#ifndef EVENT_QUEUE_H_
#define EVENT_QUEUE_H_

#include <deque>
#include <set>
#include <unordered_map>
#include <memory>
#include "event.h"

class EventTarget;

class EventQueue {
public:
  EventQueue() = default;
  virtual ~EventQueue() = default;

public:
  void push_back(const std::shared_ptr<Event> &event) {
    queue_.push_back(std::move(event));
  }

  const std::shared_ptr<Event> pop_front() {
    auto e = std::move(queue_.front());
    queue_.pop_front();
    return e;
  }

  void pop_and_broadcast();

  void register_listener(EventTarget *target) {
    listeners_.insert(target);
  }

  void unregister_listener(EventTarget *target) {
    listeners_.erase(target);
  }

  bool empty() const {
    return queue_.empty();
  }

  void clear() {
    queue_.clear();
  }

protected:
  std::deque<std::shared_ptr<Event>> queue_;
  std::set<EventTarget*> listeners_;
};

#endif /* EVENT_QUEUE_H_ */
