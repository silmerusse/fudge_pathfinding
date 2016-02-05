#ifndef FRUITCANDY_CORE_CANVAS_H_
#define FRUITCANDY_CORE_CANVAS_H_

#include <map>
#include <memory>

#include "panel.h"
#include "graphics.h"

class Canvas : public Panel {
public:
  Canvas(int w, int h): Panel(nullptr, {0, 0, w, h}) {};
  virtual ~Canvas() {
    if (window_) {
      SDL_DestroyWindow(window_);
      window_ = NULL;
    }

    SDL_Quit();
    INFO("SDL quited.");
  }

public:

  bool init(const std::string &name) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      ERROR("Unable to initialize SDL: %s.", SDL_GetError());
      return false;
    }

    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
      ERROR("Unable to initialize SDL hinting: %s.", SDL_GetError());
      return false;
    }

    if (TTF_Init() != 0) {
      ERROR("Unable to initialize TTF: %s.", TTF_GetError());
      return false;
    }

    if ((window_ = SDL_CreateWindow(name.c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        w_, h_, SDL_WINDOW_SHOWN)) == NULL) {
      ERROR("Unable to create SDL Window: %s.", SDL_GetError());
      return false;
    }

    if (!(unique_graphics_ = create_graphics())) {
      ERROR("Unable to initialize Graphics.");
      return false;
    }

    graphics_ = unique_graphics_.get();
    graphics_->saved_viewport_rect_ = get_rect();

    INFO("SDL initialized.");
    return true;
  }

  using Panel::render;

  void render() const {
    SDL_SetRenderDrawColor(graphics_->renderer_, 100, 100, 100, 255);
    SDL_RenderClear(graphics_->renderer_);
    graphics_->save_viewport();
    for (auto &component : components_) {
      if (component->enabled_ && dynamic_cast<Renderable*>(component.get())) {
        Renderable *renderable = static_cast<Renderable*>(component.get());
        auto viewport_rect = renderable->get_absolute_rect();
        graphics_->set_viewport(viewport_rect);
        renderable->render(renderable->get_boundary_box());
      }
    }
    graphics_->restore_viewport();
    SDL_RenderPresent(graphics_->renderer_);
  }

  Graphics* get_graphics() {
    return graphics_;
  }

private:
  std::unique_ptr<Graphics> create_graphics() {
    SDL_Renderer *renderer =
        SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
      ERROR("Unable to create renderer.");
      return nullptr;
    }
    return std::unique_ptr<Graphics>(new Graphics(renderer));

  }

private:
  SDL_Window *window_ = nullptr;
  std::unique_ptr<Graphics> unique_graphics_ = nullptr;
};

#endif /* FRUITCANDY_CORE_CANVAS_H_ */
