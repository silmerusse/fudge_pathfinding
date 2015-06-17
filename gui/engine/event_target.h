#ifndef EVENT_TARGET_H_
#define EVENT_TARGET_H_

#include <map>
#include <string>
#include "event_queue.h"

class Realm;

class EventTarget {
public:
  EventTarget() = default;
  virtual ~EventTarget() = default;

public:
  virtual void handle_events(Realm *realm);

  virtual void register_handler(const std::string &name,
      std::function<void(Realm*, const std::shared_ptr<Event>&)> handler);

  virtual void post_event(const std::shared_ptr<Event>& e);

private:
  EventQueue event_queue_;
  std::map<const std::string, std::function<void(Realm*,
      const std::shared_ptr<Event>&)>> handlers_;
};

#endif /* EVENT_TARGET_H_ */
