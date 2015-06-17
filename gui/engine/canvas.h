#ifndef CANVAS_H_
#define CANVAS_H_

#include <map>
#include <memory>
#include <tuple>
#include <SDL2/SDL.h>
#include "renderable.h"
#include "panel.h"
#include "texture.h"

class Canvas {
public:
  Canvas() = default;
  virtual ~Canvas();

public:
  bool init(const std::string &name, int width, int height);
  void render();
  void add_panel(std::unique_ptr<Panel> &&panel);

public:
  void dispatch_event(const SDL_Event &e);

public:
  SDL_Window *window_ = nullptr;
  SDL_Renderer *renderer_ = nullptr;

private:
  std::vector<std::unique_ptr<Panel>> panels_;
};

#endif /* CANVAS_H_ */
