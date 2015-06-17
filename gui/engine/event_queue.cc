#include "event_queue.h"
#include "event_target.h"

void EventQueue::pop_and_broadcast() {
  if (!empty()) {
    const std::shared_ptr<Event> e = pop_front();
    for (auto &listener : listeners_)
      listener->post_event(e);
  }
}
