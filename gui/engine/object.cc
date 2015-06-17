#include "object.h"
#include "event_queue.h"
#include "realm.h"

void Object::add_component(std::unique_ptr<Object> &&object) {
  object->parent_ = this;
  components_.push_back(std::move(object));
}

void Object::set_position(const Vector2D<double> &pos) {
  transform_->position_ = {pos.x, pos.y};
}

// Get relative position to parent.
const Vector2D<double> Object::get_position() const {
  return transform_->position_;
}

// Traverse parents to determine the actual position.
const Vector2D<double> Object::get_absolute_position() const {
  Object *p = parent_;
  Vector2D<double> pos = transform_->position_;
  while (p) {
    pos.rotate(p->transform_->rotation_);
    pos += p->transform_->position_;
    p = p->parent_;
  }
  return pos;
}

int Object::get_absolute_rotation() {
  Object *p = parent_;
  int rotation = transform_->rotation_;
  while (p) {
    rotation += p->transform_->rotation_;
    p = p->parent_;
  }
  return rotation;
}

void Object::move(const Vector2D<double> &offset) {
  auto pos = get_position();
  set_position({pos.x + offset.x, pos.y + offset.y});
}

void Object::rotate(int degree) {
  transform_->rotation_ += degree;
}

void Object::rotate_to(int degree) {
  transform_->rotation_ = degree;
}

const std::string Object::to_string() const {
  std::ostringstream ss;
  ss << "Transform: (" << transform_->to_string() << ")";
  return ss.str();
}
