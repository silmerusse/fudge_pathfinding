#ifndef FRUITCANDY_CORE_REALM_H_
#define FRUITCANDY_CORE_REALM_H_

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

  virtual bool initialize(const std::string &path) {
    if (!load_assets(path)) {
      ERROR("Failed to load assets.");
      return false;
    }

    if (!load_objects(path)) {
      ERROR("Failed to load objects.");
      return false;
    }

    if (!initialize_assets()) {
      ERROR("Failed to initialize assets.");
      return false;
    }

    if (!initialize_objects()) {
      ERROR("Failed to initialize objects.");
      return false;
    }
    
    if (!register_objects()) {
      ERROR("Failed to register objects.");
      return false;
    }

    return true;
  }

protected:
 	virtual bool load_objects(const std::string &path) {return true;}
  virtual bool load_assets(const std::string &path) {return true;}
  virtual bool initialize_assets() {return true;}
  virtual bool initialize_objects() {return true;}
  virtual bool register_objects() {return true;}

public:
  unsigned int w_ = 0;
  unsigned int h_ = 0;
  std::unique_ptr<EventTarget> event_target_ = nullptr;
  std::unique_ptr<Repository> repo_ = nullptr;
};

#endif /* FRUITCANDY_CORE_REALM_H_ */
