#ifndef PANEL_H_
#define PANEL_H_

#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Panel {
public:
  Panel(const SDL_Rect &rect, int z_order = 0);
  virtual ~Panel() = default;

public:
  virtual void update() {}

  virtual void render(SDL_Renderer *renderer) const;
  virtual void handle_event(const SDL_Event &e) {}

  // SDL_PointInRect is supported in SDL 2.0.4
  bool point_in_rect(const SDL_Point &point, const SDL_Rect &rect);

public:
  SDL_Rect rect_{0, 0, 0, 0}; // relative to canvas (screen)
  int z_order_ = 0;

protected:
  virtual void handle_mouse_button_down(Uint8 button, const SDL_Point& point) {}
  virtual void handle_key_down(SDL_Keycode sym) {}
};

#endif /* PANEL_H_ */
