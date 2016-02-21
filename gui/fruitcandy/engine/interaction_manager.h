#ifndef FRUITCANDY_ENGINE_INTERACTION_MANAGER_H_
#define FRUITCANDY_ENGINE_INTERACTION_MANAGER_H_

#include "event_target.h"
#include "object.h"
#include "realm.h"

class InteractionManager {
public:
  InteractionManager(Realm *realm) : realm_(realm) {}
  virtual ~InteractionManager() = default;

public:
  virtual void handle_mouse_button_down(unsigned int button, int x, int y) {}
  virtual void handle_mouse_motion(int x, int y, int xrel, int yrel) {}
  virtual void handle_key_down(int keycode, bool repeat) {}
  virtual void handle_key_up(int keycode) {}

public:
  Realm *realm_ = nullptr;
};

#endif /* FRUITCANDY_ENGINE_INTERACTION_MANAGER_H_ */
