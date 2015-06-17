#ifndef AIRBORNE_H_
#define AIRBORNE_H_

#include <memory>
#include "object.h"
#include "sprite.h"

class Realm;

class Airborne : public Object {
public:
  Airborne();
  virtual ~Airborne() = default;

public:
  static std::unique_ptr<Airborne> create() {
    return std::move(std::unique_ptr<Airborne>(new Airborne()));
  }

  void on_move_to(Realm *realm, const std::shared_ptr<Event> &e);

  void on_drop_to(Realm *realm, const std::shared_ptr<Event> &e);

  bool sprite_visible() const {
    return static_cast<Sprite*>(components_[0].get())->visible_;
  }

  void sprite_visible(bool visible) {
    static_cast<Sprite*>(components_[0].get())->visible_ = visible;
  }

};

#endif /* AIRBORNE_H_ */
