#ifndef OBJECT_H_
#define OBJECT_H_

#include <memory>
#include <vector>
#include <sstream>
#include "vector2D.h"
#include "transform.h"
#include "event_target.h"

class Realm;
class EventQueue;

class Object: public EventTarget {
public:
  Object(): transform_(new Transform()) {}
  virtual ~Object() = default;

public:
  // Add a child object.
  void add_component(std::unique_ptr<Object> &&object);

  // Set relative position to parent.
  void set_position(const Vector2D<double> &pos);

  // Get relative position to parent.
  const Vector2D<double> get_position() const;

  // Traverse parents to determine the actual position.
  const Vector2D<double> get_absolute_position() const;

  // Get absolute position to root.
  int get_absolute_rotation();

  // Move by an offset.
  void move(const Vector2D<double> &offset);

  // Rotate by a degree.
  void rotate(int degree);

  // Rotate to the degree specified.
  void rotate_to(int degree);

public:
  // This could be overridden by derived class to added additional behavior.
  virtual void tick(Realm *realm) {
    handle_events(realm);
  }

  // Represent as a string.
  virtual const std::string to_string() const;

public:
  // Factory method.
  static std::unique_ptr<Object> create() {
    return std::move(std::unique_ptr <Object> (new Object()));
  }

public:
  std::vector<std::unique_ptr<Object>> components_; // Container for children.
  Object *parent_ = nullptr; // Point to parent.
  bool selectable_ = false;  // Flag to indicate if this is selectable.
  bool selected_ = false;    // Flag to indicate if this has been selected.

protected:
  std::unique_ptr<Transform> transform_ = nullptr;
};

#endif /* OBJECT_H_ */
