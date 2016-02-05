#ifndef FRUITCANDY_MATH_TRANSFORM_H_
#define FRUITCANDY_MATH_TRANSFORM_H_

#include <sstream>
#include "vector2D.h"

class Transform {
public:
  Transform() {};
  Transform(const Vector2D<double> &position,
  		      const Vector2D<double> &scale = {1.0, 1.0},
  		      double rotation = 0.0)
      :	position_(position), scale_(scale), rotation_(rotation) {}
  virtual ~Transform() = default;

public:
  // Set relative position to parent.
  void set_position(const Vector2D<double> &pos) {
      position_ = {pos.x, pos.y};
  }

  // Get relative position to parent.
  const Vector2D<double> get_position() const {
    return position_;
  }

  // Move by an offset.
  void move(const Vector2D<double> &offset) {
    auto pos = get_position();
    set_position({pos.x + offset.x, pos.y + offset.y});
  }

  // Rotate by a degree.
  void rotate(int degree) {
    rotation_ += degree;
  }

  // Rotate to the degree specified.
  void rotate_to(int degree) {
    rotation_ = degree;
  }

  const std::string to_string() const {
    std::ostringstream ss;
    ss << "Position:" << position_.to_string() << ";Rotation:"
        << rotation_ << ";Scale:" << scale_.to_string();
    return ss.str();
  }

public:
  Vector2D<double> position_ {0.0, 0.0};
  Vector2D<double> scale_ {1.0, 1.0};
  double rotation_  = 0.0;
};

#endif /* FRUITCANDY_CORE_TRANSFORM_H_ */
