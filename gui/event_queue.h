#ifndef EVENT_QUEUE_H_
#define EVENT_QUEUE_H_

#include <deque>
#include <set>
#include <unordered_map>
#include <memory>
#include "event.h"
#include "object.h"

class EventQueue {
public:
  EventQueue() = default;
  virtual ~EventQueue() = default;

public:
  void push_back(const std::shared_ptr<Event> &event) {
    queue_.push_back(std::move(event));
  }

  std::shared_ptr<Event> pop_front() {
    auto e = std::move(queue_.front());
    queue_.pop_front();
    return e;
  }

  void pop_front_notify_listeners() {
    std::shared_ptr<Event> e = pop_front();
    for (auto &listener : listeners_)
      listener->event_queue_->push_back(e);
  }

  void register_listener(Object *object) {
    listeners_.insert(object);
  }

  void unregister_listener(Object *object) {
    listeners_.erase(object);
  }

  bool empty() const {
    return queue_.empty();
  }

  void clear() {
    queue_.clear();
  }

protected:
  std::deque<std::shared_ptr<Event>> queue_;
  std::set<Object*> listeners_;
};

class MultiEventQueue {
public:
  MultiEventQueue() = default;
  virtual ~MultiEventQueue() = default;

public:
  void push_back(const std::shared_ptr<Event> &event) {
    const std::string type = event->type_;
    queues_[type].push_back(event);
    ++size_;
  }

  void register_listener(const std::string &type, Object *object) {
    queues_[type].register_listener(object);
  }

  void unregister_listener(Object *object) {
    for (auto &p : queues_) {
      p.second.unregister_listener(object);
    }
  }

  void pop_front_notify_listeners() {
    for (auto &p : queues_) {
      p.second.pop_front_notify_listeners();
    }
    --size_;
  }

  bool empty() const {
    return size_ == 0;
  }

protected:
  std::map<const std::string, EventQueue> queues_;
  size_t size_ = 0;
};

#endif /* EVENT_QUEUE_H_ */
