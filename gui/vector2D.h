#ifndef VECTOR2D_H_
#define VECTOR2D_H_

#include <math.h>
#include <utility>
#include <string>
#include <sstream>

constexpr double kPI  =3.141592653589793238463;

template<typename T> class Vector2D {
public:
  T x;
  T y;

public:
  Vector2D(const T& x = 0, const T& y = 0) :
      x(x), y(y) {
  }

  Vector2D(const Vector2D<T>& src) :
      x(src.x), y(src.y) {
  }

  Vector2D(const std::pair<T, T>& p) :
      x(p.first), y(p.second) {
  }

  virtual ~Vector2D() {}

  std::pair<T, T> constexpr to_pair() const {
    return std::pair<T, T> {x, y};
  }

  inline constexpr T X() const {
    return x;
  }

  inline constexpr T Y() const {
    return y;
  }

  inline void X(const T& x) {
    this->x = x;
  }

  inline void Y(const T& y) {
    this->y = y;
  }

  inline constexpr Vector2D<T> operator -() const {
    return Vector2D<T>(-x, -y);
  }

  inline constexpr Vector2D<T> operator +() const {
    return Vector2D<T>(+x, +y);
  }

  inline constexpr Vector2D<T> operator +(const Vector2D<T>& v) const {
    return Vector2D<T>(x + v.x, y + v.y);
  }

  inline Vector2D<T>& operator +=(const Vector2D<T>& v) {
    x += v.x;
    y += v.y;
    return *this;
  }

  inline constexpr Vector2D<T> operator -(const Vector2D<T>& v) const {
    return Vector2D<T>(x - v.x, y - v.y);
  }

  inline Vector2D<T>& operator -=(const Vector2D<T>& v) {
    x -= v.x;
    y -= v.y;
    return *this;
  }

  inline constexpr Vector2D<T> operator *(const T& s) const {
    return Vector2D<T>(x * s, y * s);
  }

  inline constexpr T operator *(const Vector2D<T>& v) const {
    return x * v.x + y * v.y;
  }

  inline constexpr bool operator ==(const Vector2D<T>& v) const {
     return x == v.x && y == v.y;
  }

  inline constexpr bool operator !=(const Vector2D<T>& v) const {
     return !operator==(v);
  }

  inline constexpr T norm() const {
    return sqrt(x * x + y * y);
  }

  // Inner angle (degree) of two vectors in clockwise (v - this)
  inline constexpr T angle(const Vector2D<T>& v) const {
    // atan2(y, x) or atan2(sin, cos)
    return (atan2(y, x) - atan2(v.y, v.x)) / kPI * 180;
  }

  // Rotate in clockwise by theta angle (degree) in left-handed Cartesian
  // coordinate system (screen coordinate).
  inline void rotate(const T theta) {
    double t = theta / 180.0 * kPI;
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
};

#endif /* VECTOR2D_H_ */
