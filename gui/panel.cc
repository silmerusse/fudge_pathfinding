#include "panel.h"
#include "canvas.h"

Panel::Panel(const SDL_Rect &rect, int z_order) :
    rect_(rect), z_order_(z_order) {
}

bool Panel::point_in_rect(const SDL_Point &point, const SDL_Rect &rect) {
  return point.x > rect.x && point.x < rect.x + rect.w && point.y > rect.y
      && point.y < rect.y + rect.h;
}

void Panel::render(SDL_Renderer *renderer) const {
  // Transform drawing coordinates and set drawing area.
  SDL_RenderSetViewport(renderer, &rect_);

  // Draw border
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

  SDL_Rect r{0, 0, rect_.w, rect_.h};
  SDL_RenderDrawRect(renderer, &r);
}

