#ifndef FRUITCANDY_CORE_EVENT_TARGET_H_
#define FRUITCANDY_CORE_EVENT_TARGET_H_

#include <map>
#include <string>
#include <deque>
#include <set>
#include <memory>

#include "event.h"

class EventTarget {
public:
  EventTarget() = default;
  virtual ~EventTarget() = default;

public:
  virtual void handle_events() {
    while (!event_queue_.empty()) {
      auto event = event_queue_.front();
      event_queue_.pop_front();

      auto i = handlers_.find(event->type_);
      if (i != handlers_.end()) {
        i->second(event);
      } else {
        forward_event(event);
      }
    }
  }

  virtual void forward_event(const std::shared_ptr<Event> &event) {
    auto i = listeners_.find(event->type_);
    if (i != listeners_.end()) {
      for (auto listener : i->second)
        listener->post_event(event);
    }
  }

  virtual void register_handler(const std::string &name,
      std::function<void(const std::shared_ptr<Event>&)> handler) {
    handlers_[name] = handler;
  }

  virtual void post_event(const std::shared_ptr<Event>& e) {
    event_queue_.push_back(e);
  }

  virtual void register_listener(const std::string &name,
                                 EventTarget *target) {
    listeners_[name].insert(target);
  }

  virtual void unregister_listener(const std::string &name,
                                   EventTarget *target) {
    if (listeners_.find(name) != listeners_.end())
      listeners_[name].erase(target);
  }

protected:
  std::deque<std::shared_ptr<Event>> event_queue_;
  std::map<std::string, std::set<EventTarget*>> listeners_;
  std::map<std::string,
           std::function<void(const std::shared_ptr<Event>&)>> handlers_;
};

#endif /* FRUITCANDY_CORE_EVENT_TARGET_H_ */
