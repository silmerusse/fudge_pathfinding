#ifndef OBJECT_H_
#define OBJECT_H_

#include <memory>
#include <vector>
#include <sstream>
#include "loadable.h"
#include "transform.h"
#include "event.h"

class Realm;
class EventQueue;

class Object: public Loadable {
public:
  Object();
  virtual ~Object();

public:
  void add_component(std::unique_ptr<Object> &&object) {
    object->parent_ = this;
    components_.push_back(std::move(object));
  }

public:
  // Set relative position to parent.
  void set_position(const Vector2D<double> &pos) {
    transform_->position_ = pos;
  }

  // Get relative position to parent.
  const Vector2D<double> get_position() const {
    return transform_->position_;
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

  int get_absolute_rotation() {
    Object *p = parent_;
    int rotation = transform_->rotation_;
    while (p) {
      rotation += p->transform_->rotation_;
      p = p->parent_;
    }
    return rotation;
  }

  void move(const Vector2D<double> &delta) {
    auto pos = get_position();
    set_position({pos.x + delta.x, pos.y + delta.y});
  }

  void rotate(int degree) {
    transform_->rotation_ += degree;
  }

  void rotate_to(int degree) {
    transform_->rotation_ = degree;
  }

  static std::unique_ptr<Object> create() {
    return std::move(std::unique_ptr <Object> (new Object()));
  }

  virtual void tick(Realm *realm);

  virtual void handle_events(Realm *realm);

  virtual void on_mouse_button_down(Realm *realm, unsigned int button,
                                    int x, int y);

  virtual void on_mouse_motion(Realm *realm, int x, int y, int relx, int rely);

  virtual void on_key_down(Realm *realm, int sym);

  virtual const std::string to_string() const {
    std::ostringstream ss;
    ss << "Transform: (" << transform_->to_string() << ")";
    return ss.str();

  }

public:
  std::vector<std::unique_ptr<Object>> components_;
  Object *parent_ = nullptr;
  bool selected_ = false;
  bool selectable_ = false;
  std::unique_ptr<EventQueue> event_queue_;

protected:
  std::unique_ptr<Transform> transform_ = nullptr;
};

#endif /* OBJECT_H_ */
