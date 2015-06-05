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
      int x = event->params_[0].asInt();
      int y = event->params_[1].asInt();
      on_mouse_button_down(realm, x, y);
    }
  }
}

void Object::on_mouse_button_down(Realm *realm, int x, int y) {
  auto pos = get_absolute_position();
  if (abs(static_cast<int>(x - pos.x)) <= 16 &&
      abs(static_cast<int>(y - pos.y)) <= 16) {
    realm->select(this);
    DEBUG("Unit selected: %s", to_string().c_str());
  } else {
    if (realm->selectee_ == this) {
      realm->unselect();
      DEBUG("Unit unselected: %s", to_string().c_str());
    }
  }
}
