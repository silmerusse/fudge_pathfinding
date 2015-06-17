#ifndef EVENT_HUB_H_
#define EVENT_HUB_H_

#include "event_queue.h"

class EventHub {
public:
  EventHub() = default;
  virtual ~EventHub() = default;

public:
  void push_back(const std::shared_ptr<Event> &event) {
    const std::string type = event->type_;
    queues_[type].push_back(event);
    ++size_;
  }

  void register_listener(const std::string &type, EventTarget *target) {
    queues_[type].register_listener(target);
  }

  void unregister_listener(EventTarget *target) {
    for (auto &p : queues_) {
      p.second.unregister_listener(target);
    }
  }

  void pop_broadcast() {
    for (auto &p : queues_) {
      p.second.pop_and_broadcast();
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

#endif /* ENGINE_EVENT_HUB_H_ */
