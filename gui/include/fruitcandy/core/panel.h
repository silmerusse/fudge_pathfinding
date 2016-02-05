#ifndef FRUITCANDY_CORE_PANEL_H_
#define FRUITCANDY_CORE_PANEL_H_

#include <memory>
#include <vector>

#include "../math/point_in_rect.h"
#include "../math/vector2D.h"

#include "graphics.h"
#include "renderable.h"

class Panel : public Renderable {
public:
  Panel(Graphics* graphics, const SDL_Rect &rect, int z_order = 0)
      : Renderable(graphics, rect.w, rect.h, z_order) {
    transform_->set_position({rect.x, rect.y});
    event_target_.reset(new EventTarget());
  }

  virtual ~Panel() = default;

public:
  virtual void render(const SDL_Rect &rect) const override {
    if (graphics_) {
      // Draw border
      SDL_SetRenderDrawColor(graphics_->renderer_,
                             255, 255, 255, SDL_ALPHA_OPAQUE);

      SDL_RenderDrawRect(graphics_->renderer_, &rect);

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
    } else {
      ERROR("Panel: Graphics not found.");
    }
  }

  virtual const SDL_Rect get_rect() const override {
      return SDL_Rect{static_cast<int>(transform_->get_position().x),
                      static_cast<int>(transform_->get_position().y),
                      w_, h_};
  }

  virtual const SDL_Rect get_absolute_rect() const override {
    auto pos = get_absolute_position();
    return SDL_Rect{static_cast<int>(pos.x),
                    static_cast<int>(pos.y),
                    w_, h_};
  }

  virtual void dispatch_event(const SDL_Event &e) {
    switch (e.type) {
    case SDL_MOUSEBUTTONDOWN:
      DEBUG("Panel mouse button down: (%d, %d)", e.button.x, e.button.y);
      for (auto &component : components_) {
        if (component->enabled_ && dynamic_cast<Renderable*>(component.get())) {
          auto renderable = static_cast<Renderable*>(component.get());
          auto rect = renderable->get_rect();
          if (point_in_rect(SDL_Point{e.button.x, e.button.y}, rect)) {
            SDL_Event e1 = e;
            e1.button.x -= rect.x;
            e1.button.y -= rect.y;
            if (dynamic_cast<Panel*>(renderable)) {
              static_cast<Panel*>(renderable)->dispatch_event(e1);
            }
            break;
          }
        }
      }
      break;
    default:
      for (auto &component : components_) {
        if (component->enabled_ && dynamic_cast<Panel*>(component.get())) {
          static_cast<Panel*>(component.get())->dispatch_event(e);
        }
      }
    }
  }

  virtual void add_panel(std::unique_ptr<Panel> panel) {
    add_child(std::move(panel));

    std::sort(components_.begin(), components_.end(),
              [](const std::unique_ptr<Object> &o0,
                 const std::unique_ptr<Object> &o1) -> bool {
                return static_cast<Renderable*>(o0.get())->z_order_ <
                    static_cast<Renderable*>(o1.get())->z_order_;
              });
  }

  virtual void update() {
    for (auto &component : components_) {
      if (component->enabled_ && dynamic_cast<Panel*>(component.get())) {
        static_cast<Panel*>(component.get())->update();
      }
    };
  }

  virtual void scale(double s) {
    w_ = static_cast<int>(w_ * s);
    h_ = static_cast<int>(h_ * s);
  }
};

#endif /* FRUITCANDY_CORE_PANEL_H_ */
