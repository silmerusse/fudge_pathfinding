#ifndef FRUITCANDY_CORE_EVENT_HANDLER_H_
#define FRUITCANDY_CORE_EVENT_HANDLER_H_

#include "event_target.h"
#include "object.h"
#include "realm.h"

class EventHandler {
public:
  EventHandler(Realm *realm) : realm_(realm) {}
  virtual ~EventHandler() = default;

public:
  virtual void handle_mouse_button_down(unsigned int button, int x, int y) {}
  virtual void handle_mouse_motion(int x, int y, int xrel, int yrel) {}
  virtual void handle_key_down(int keycode, bool repeat) {}
  virtual void handle_key_up(int keycode) {}

public:
  Realm *realm_ = nullptr;
};

#endif /* FRUITCANDY_CORE_EVENT_HANDLER_H_ */
