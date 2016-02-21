#ifndef FRUITCANDY_ENGINE_REALM_H_
#define FRUITCANDY_ENGINE_REALM_H_

#include <memory>

#include "../util/log.h"
#include "repository.h"
#include "event_target.h"
#include "object.h"

class Realm {
public:
  explicit Realm(const unsigned int w, const unsigned int h): w_(w), h_(h) {
    event_target_.reset(new EventTarget());
    repo_.reset(new Repository(this));
  };
  virtual ~Realm() = default;

  virtual void handle_events() {
    if (event_target_)
      event_target_->handle_events();
  }

  virtual void tick(unsigned long ticks) {
    handle_events();
    if (repo_) {
      repo_->traverse_all([&](Item *item) {
        static_cast<Object*>(item)->tick(ticks);
      });
    }
  }

public:
  unsigned int w_ = 0;
  unsigned int h_ = 0;
  std::unique_ptr<EventTarget> event_target_ = nullptr;
  std::unique_ptr<Repository> repo_ = nullptr;
};

#endif /* FRUITCANDY_ENGINE_REALM_H_ */
