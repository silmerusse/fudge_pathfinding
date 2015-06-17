#ifndef PANEL_H_
#define PANEL_H_

#include <memory>
#include <SDL2/SDL.h>

class Panel {
public:
  Panel(const SDL_Rect &rect, int z_order = 0);
  virtual ~Panel() = default;

public:
  virtual void update() {}
  virtual void render(SDL_Renderer *renderer) const;
  virtual void dispatch_event(const SDL_Event &e) {}

public:
  SDL_Rect rect_{0, 0, 0, 0}; // relative to canvas (screen)
  int z_order_ = 0;
};

#endif /* PANEL_H_ */
