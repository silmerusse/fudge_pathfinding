#ifndef REALM_H_
#define REALM_H_

#include <memory>
#include "log.h"
#include "object_manager.h"
#include "event_queue.h"

class Realm {
public:
  explicit Realm(const unsigned int w, const unsigned int h): w_(w), h_(h) {};
  virtual ~Realm() = default;

  void select(Object *selectee) {
    if (selectee_)
      selectee_->selected_ = false;
    selectee->selected_ = true;
    selectee_ = selectee;
  }

  void unselect() {
    if (selectee_)
      selectee_->selected_ = false;
    selectee_ = nullptr;
  }

  void handle_events() {
    while (!event_queue_.empty()) {
      event_queue_.pop_front_notify_listeners();
    }
  }

  void tick() {
    handle_events();
    object_manager_->traverse_all([&](Object *object) {
      object->tick(this);
    });
  }

public:
  unsigned int w_ = 0;
  unsigned int h_ = 0;
  std::unique_ptr<ObjectManager> object_manager_ = nullptr;
  Object *selectee_ = nullptr;
  MultiEventQueue event_queue_;
};

#endif /* REALM_H_ */
