#ifndef INTERACTION_MANAGER_H_
#define INTERACTION_MANAGER_H_

#include <SDL2/SDL.h>
#include "event_target.h"
#include "object.h"

class Realm;

class InteractionManager : public EventTarget {
public:
  InteractionManager() {
    register_handler("mouse_button_down",
        std::bind(&InteractionManager::on_mouse_button_down,
            this, std::placeholders::_1, std::placeholders::_2));

    register_handler("mouse_motion",
        std::bind(&InteractionManager::on_mouse_motion,
            this, std::placeholders::_1, std::placeholders::_2));

    register_handler("key_down",
        std::bind(&InteractionManager::on_key_down,
            this, std::placeholders::_1, std::placeholders::_2));
  }
  virtual ~InteractionManager() = default;

public:
  virtual void on_mouse_button_down(Realm *realm,
      const std::shared_ptr<Event> &e);

  virtual void on_mouse_motion(Realm *realm,
      const std::shared_ptr<Event> &e);

  virtual void on_key_down(Realm *realm,
      const std::shared_ptr<Event> &e);

public:
  // Track selected object.
  Object *selectee_ = nullptr;

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

private:
  // Track the mouse position.
  int x_ = 0;
  int y_ = 0;
};

#endif /* INTERACTION_MANAGER_H_ */
