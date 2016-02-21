#ifndef FRUITCANDY_MATH_POINT_IN_RECT_H_
#define FRUITCANDY_MATH_POINT_IN_RECT_H_

#include <SDL2/SDL.h>

// SDL_PointInRect is not supported before SDL 2.0.4.
constexpr bool point_in_rect(const SDL_Point &point, const SDL_Rect &rect) {
  return point.x > rect.x && point.x < rect.x + rect.w &&
  		   point.y > rect.y && point.y < rect.y + rect.h;
}

#endif /* FRUITCANDY_MATH_POINT_IN_RECT_H_ */
