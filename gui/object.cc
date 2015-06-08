#include "object.h"
#include "event_queue.h"
#include "realm.h"

Object::Object():
    Loadable(), event_queue_(new EventQueue()), transform_(new Transform()) {}

Object::~Object() {}

void Object::tick(Realm *realm) {
  handle_events(realm);
}

void Object::handle_events (Realm *realm) {
  while (!event_queue_->empty()) {
    auto event = event_queue_->pop_front();

    if (event->type_ == "mouse_button_down") {
      on_mouse_button_down(
          realm, event->params_[0].asUInt(), event->params_[1].asInt(),
          event->params_[2].asInt());
    } else if (event->type_ == "mouse_motion" ) {
      on_mouse_motion(
          realm, event->params_[0].asInt(), event->params_[1].asInt(),
          event->params_[2].asInt(), event->params_[3].asInt());
    } else if (event->type_ == "key_down") {
      on_key_down(realm, event->params_[0].asInt());
    }
  }
}

void Object::on_mouse_button_down(Realm *realm, unsigned int button,
                                  int x, int y) {}

void Object::on_mouse_motion(Realm *realm, int x, int y, int relx, int rely) {}

void Object::on_key_down(Realm *realm, int sym) {}
