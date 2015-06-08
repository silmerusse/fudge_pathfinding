#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include <sstream>
#include "vector2D.h"

class Transform {
public:
  Transform() = default;
  virtual ~Transform() = default;

public:
  const std::string to_string() const {
    std::ostringstream ss;
    ss << "Position:" << position_.to_string() << ";Rotation:"
        << rotation_ << ";Scale:" << scale_.to_string();
    return ss.str();
  }

public:
  Vector2D<double> position_ {0.0, 0.0};
  double rotation_  = 0;
  Vector2D<double> scale_ {1.0, 1.0};
};

#endif /* TRANSFORM_H_ */
