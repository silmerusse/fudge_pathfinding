#ifndef FUDGE_PATHFINDING_GUI_OBJECT_AIRBORNE_H_
#define FUDGE_PATHFINDING_GUI_OBJECT_AIRBORNE_H_

#include <memory>

#include "fruitcandy/engine/object.h"
#include "fruitcandy/renderable/sprite.h"
#include "unit.h"

class Realm;

class Airborne : public Unit {
public:
  Airborne();
  virtual ~Airborne() = default;

public:
  static std::unique_ptr<Airborne> create() {
    return std::move(std::unique_ptr<Airborne>(new Airborne()));
  }

  void on_move_to(const std::shared_ptr<Event> &e);

  void on_drop_to(const std::shared_ptr<Event> &e);

  bool sprite_visible() const {
    return static_cast<Sprite*>(components_[0].get())->visible_;
  }

  void sprite_visible(bool visible) {
    static_cast<Sprite*>(components_[0].get())->visible_ = visible;
  }

};

#endif /* FUDGE_PATHFINDING_GUI_OBJECT_AIRBORNE_H_ */
