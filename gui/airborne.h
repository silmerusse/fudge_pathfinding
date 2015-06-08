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

  virtual void on_mouse_button_down(Realm *realm, unsigned int button,
                                    int x, int y) override;

  virtual void on_mouse_motion(Realm *realm,
                               int x, int y, int relx, int rely) override;

  virtual void on_key_down(Realm *realm, int sym) override;

  bool sprite_visible() const {
    return static_cast<Sprite*>(components_[0].get())->visible_;
  }

  void sprite_visible(bool visible) {
    static_cast<Sprite*>(components_[0].get())->visible_ = visible;
  }

};

#endif /* AIRBORNE_H_ */
