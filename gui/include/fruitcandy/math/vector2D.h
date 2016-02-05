#ifndef FRUITCANDY_MATH_VECTOR2D_H_
#define FRUITCANDY_MATH_VECTOR2D_H_

#include <math.h>
#include <utility>
#include <string>
#include <sstream>
#include <initializer_list>

template<typename T> class Vector2D {
public:
  Vector2D(): x(0), y(0) {}
  Vector2D(const T &x, const T &y): x(x), y(y) {}
  Vector2D(const Vector2D<T> &src): x(src.x), y(src.y) {}
  Vector2D(const std::pair<T, T> &p): x(p.first), y(p.second) {}

  Vector2D(std::initializer_list<int> l) {
    if (l.size() == 2) {
      auto i = l.begin();
      x = static_cast<T>(*i++);
      y = static_cast<T>(*i);
    }
  }

  Vector2D(std::initializer_list<double> l) {
    if (l.size() == 2) {
      auto i = l.begin();
      x = static_cast<T>(*i++);
      y = static_cast<T>(*i);
    }
  }

  virtual ~Vector2D() {}

public:
  T x;
  T y;

public:
  std::pair<T, T> constexpr to_pair() const {
    return std::pair<T, T> {x, y};
  }

  constexpr T X() const {
    return x;
  }

  constexpr T Y() const {
    return y;
  }

  void X(const T &x) {
    this->x = x;
  }

  void Y(const T &y) {
    this->y = y;
  }

  constexpr Vector2D<T> operator -() const {
    return Vector2D<T>(-x, -y);
  }

  constexpr Vector2D<T> operator +() const {
    return Vector2D<T>(+x, +y);
  }

  constexpr Vector2D<T> operator +(const Vector2D<T> &v) const {
    return Vector2D<T>(x + v.x, y + v.y);
  }

  Vector2D<T>& operator +=(const Vector2D<T> &v) {
    x += v.x;
    y += v.y;
    return *this;
  }

  constexpr Vector2D<T> operator -(const Vector2D<T> &v) const {
    return Vector2D<T>(x - v.x, y - v.y);
  }

  Vector2D<T>& operator -=(const Vector2D<T> &v) {
    x -= v.x;
    y -= v.y;
    return *this;
  }

  constexpr Vector2D<T> operator *(const T &s) const {
    return Vector2D<T>(x * s, y * s);
  }

  // Dot multiplication.
  constexpr T operator *(const Vector2D<T> &v) const {
    return x * v.x + y * v.y;
  }

  constexpr bool operator ==(const Vector2D<T> &v) const {
     return x == v.x && y == v.y;
  }

  constexpr bool operator !=(const Vector2D<T> &v) const {
     return !operator==(v);
  }

  constexpr T norm() const {
    return sqrt(x * x + y * y);
  }

  // Inner angle (degree) of two vectors in clockwise (v - this)
  constexpr T angle(const Vector2D<T> &v) const {
    // atan2(y, x) or atan2(sin, cos)
    return (atan2(y, x) - atan2(v.y, v.x)) / pi() * 180;
  }

  void clip(const Vector2D<T> &lower, const Vector2D<T> &upper) {
    x = std::max(lower.x, std::min(x, upper.x));
    y = std::max(lower.x, std::min(y, upper.y));
  }

  // Rotate in clockwise by theta angle (degree) in left-handed Cartesian
  // coordinate system (screen coordinate).
  void rotate(const T theta) {
    double t = theta / 180.0 * pi();
    double tx = cos(t) * x - sin(t) * y;
    double ty = sin(t) * x + cos(t) * y;
    x = tx;
    y = ty;
  }

  const std::string to_string() const {
    std::ostringstream ss;
    ss << x << ',' << y;
    return ss.str();
  }

private:
  static constexpr double pi() {
    return 3.141592653589793238463;
  }
};

#endif /* FRUITCANDY_CORE_VECTOR2D_H_ */
