#ifndef REALM_H_
#define REALM_H_

#include <memory>
#include "log.h"
#include "object_manager.h"
#include "module.h"
#include "event_hub.h"

class Realm {
public:
  explicit Realm(const unsigned int w, const unsigned int h): w_(w), h_(h) {
    object_manager_.reset(new ObjectManager());
  };
  virtual ~Realm() = default;

  virtual void handle_events() {
    while (!event_hub_.empty()) {
      event_hub_.pop_broadcast();
    }
  }

  virtual void tick() {
    handle_events();
    if (object_manager_) {
      object_manager_->traverse_all([&](Object *object) {
        object->tick(this);
      });
    }
  }

public:
  unsigned int w_ = 0;
  unsigned int h_ = 0;
  EventHub event_hub_;
  std::unique_ptr<ObjectManager> object_manager_ = nullptr;
};

#endif /* REALM_H_ */
