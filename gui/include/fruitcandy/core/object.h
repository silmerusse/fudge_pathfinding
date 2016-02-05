#ifndef FRUITCANDY_CORE_OBJECT_H_
#define FRUITCANDY_CORE_OBJECT_H_

#include <memory>
#include <vector>
#include <sstream>

#include "../util/log.h"
#include "../math/vector2D.h"
#include "../math/transform.h"
#include "event_target.h"
#include "item.h"

class Object : public Item {
public:
  Object() = default;
  virtual ~Object() = default;

public:
  // Add a child object.
  void add_child(std::unique_ptr<Object> object) {
    object->parent_ = this;
    components_.push_back(std::move(object));
  }

  // Traverse parents to determine the actual position.
  const Vector2D<double> get_absolute_position() const {
    Object *p = parent_;
    Vector2D<double> pos = transform_->position_;
    while (p) {
      pos.rotate(p->transform_->rotation_);
      pos += p->transform_->position_;
      p = p->parent_;
    }
    return pos;
  }

  int get_absolute_rotation() const {
    Object *p = parent_;
    int rotation = transform_->rotation_;
    while (p) {
      rotation += p->transform_->rotation_;
      p = p->parent_;
    }
    return rotation;
  }

  // This could be overridden by derived class to added additional behavior.
  virtual void tick(unsigned long ticks) {
    if (event_target_)
      event_target_->handle_events();
  }

  // Represent as a string.
  virtual const std::string to_string() const override {
    std::ostringstream ss;
    ss << Item::to_string();

    if (transform_)
      ss << "transform: (" << transform_->to_string() << ")";
    return ss.str();
  }


public:
  std::vector<std::unique_ptr<Object>> components_; // Container for children.
  Object *parent_ = nullptr; // Point to parent.
  bool selectable_ = false;  // Flag to indicate if this is selectable.
  bool selected_ = false;    // Flag to indicate if this has been selected.
  bool enabled_ = true;

public:
  std::unique_ptr<Transform> transform_ = nullptr;
  std::unique_ptr<EventTarget> event_target_ = nullptr;
};

#endif /* FRUITCANDY_CORE_OBJECT_H_ */
