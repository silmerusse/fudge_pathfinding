#include "event_target.h"
#include "realm.h"

void EventTarget::handle_events(Realm *realm) {
  while (!event_queue_.empty()) {
    auto event = event_queue_.pop_front();

    auto i = handlers_.find(event->type_);
    if (i != handlers_.end()) {
      i->second(realm, event);
    }
  }
}

void EventTarget::register_handler(const std::string &name,
    std::function<void(Realm *, const std::shared_ptr<Event>&)> handler) {
  handlers_[name] = std::move(handler);
}

void EventTarget::post_event(const std::shared_ptr<Event>& e) {
  event_queue_.push_back(e);
}
