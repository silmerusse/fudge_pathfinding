#include <algorithm>
#include "log.h"
#include "canvas.h"
#include "point_in_rect.h"

Canvas::~Canvas() {
  if (renderer_) {
    SDL_DestroyRenderer(renderer_);
    renderer_ = NULL;
  }

  if (window_) {
    SDL_DestroyWindow(window_);
    window_ = NULL;
  }

  SDL_Quit();
  INFO("SDL quited.");
}

bool Canvas::init(const std::string &name, int width, int height) {

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    ERROR("Unable to Init SDL: %s.", SDL_GetError());
    return false;
  }

  if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
    ERROR("Unable to Init hinting: %s.", SDL_GetError());
    return false;
  }

  if ((window_ = SDL_CreateWindow(name.c_str(),
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      width, height, SDL_WINDOW_SHOWN)) == NULL) {
    ERROR("Unable to create SDL Window: %s.", SDL_GetError());
    return false;
  }

  if ((renderer_ = SDL_CreateRenderer(window_, -1,
                                      SDL_RENDERER_ACCELERATED)) == NULL) {
    ERROR("Unable to create renderer.");
    return false;
  }

  SDL_SetRenderDrawColor(renderer_, 0x00, 0x00, 0x00, 0xFF);

  INFO("SDL initialized.");
  return true;
}

void Canvas::render() {
  SDL_SetRenderDrawColor(renderer_, 100, 100, 100, 255);
  SDL_RenderClear(renderer_);

  for (auto &p : panels_) {
    p->render(renderer_);
  }

  SDL_RenderPresent(renderer_);
}

void Canvas::dispatch_event(const SDL_Event &e) {
  switch (e.type) {
  case SDL_MOUSEBUTTONDOWN:
    DEBUG("Canvas mouse button down: (%d, %d)", e.button.x, e.button.y);
    for (auto &panel : panels_) {
      if (point_in_rect(SDL_Point{e.button.x, e.button.y}, panel->rect_)) {
        SDL_Event e1 = e;
        e1.button.x -= panel->rect_.x;
        e1.button.y -= panel->rect_.y;
        panel->dispatch_event(e1);
        break;
      }
    }
    break;
  default:
    for (auto &panel : panels_) {
      panel->dispatch_event(e);
    }
  }
}

void Canvas::add_panel(std::unique_ptr<Panel> &&panel) {
  panels_.push_back(std::move(panel));
  std::sort(panels_.begin(), panels_.end(),
            [](const std::unique_ptr<Panel> &panel_a,
               const std::unique_ptr<Panel> &panel_b) -> bool {
              return panel_a->z_order_ < panel_b->z_order_;
            });
}

