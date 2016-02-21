#ifndef FRUITCANDY_RENDERABLE_LINE_H_
#define FRUITCANDY_RENDERABLE_LINE_H_

#include <sstream>

#include "../engine/renderable.h"

class Line: public Renderable {
public:
  Line(int x0, int y0, int x1, int y1, Graphics *graphics, int z_order = 0)
      :	Renderable(graphics, z_order), x0_(x0), y0_(y0), x1_(x1), y1_(y1) {
		w_ = std::abs(x1 - x0) + 1;
		h_ = std::abs(y1 - y0) + 1;

		SDL_Texture *texture = SDL_CreateTexture(
				graphics->renderer_, SDL_PIXELFORMAT_RGBA8888,
				SDL_TEXTUREACCESS_TARGET, w_, h_);
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
		SDL_SetRenderTarget(graphics->renderer_, texture);
		SDL_SetRenderDrawColor(graphics->renderer_, 0, 0, 0, 0);
		SDL_RenderClear(graphics_->renderer_);
		SDL_SetRenderDrawColor(graphics_->renderer_,
													 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(graphics_->renderer_,
											 x0_ - std::min(x0, x1), y0_ - std::min(y0, y1),
											 x1_ - std::min(x0, x1), y1_ - std::min(y0, y1));
		SDL_SetRenderTarget(graphics->renderer_, NULL);
		texture_.reset(new Texture(texture));
  }

  Line(const Line &line): Renderable(line),
      texture_(std::unique_ptr<Texture>(
          new Texture(line.texture_->clone(line.graphics_->renderer_)))) {};
  virtual ~Line() {
    // The texture was created by us. We need to destroy it manually.
    if (texture_ && texture_->get_texture_ptr()) {
      SDL_DestroyTexture(texture_->get_texture_ptr());
    }
  }

public:
  virtual void render(const SDL_Rect &rect) const override {
    if (graphics_ && texture_) {
      graphics_->render_texture(texture_.get(), &rect, NULL,
                                get_absolute_rotation());
    } else {
      ERROR ("Line: Graphics not found.");
    }
  }

public:
  int x0_ = 0;
  int y0_ = 0;
  int x1_ = 0;
  int y1_ = 0;

public:
  const std::string to_string() const override {
    std::ostringstream ss;
    ss << Renderable::to_string();
    ss << '(' << x0_ << ',' << y0_ << ')' << " -> " << '(' << x1_ << ',' << y1_
       << ')';
    return ss.str();
  }

public:
  // Factory method to create a new instance.
  static std::unique_ptr<Line> create(int x0, int y0, int x1, int y1,
                                      Graphics *graphics, int z_order = 0) {
    return std::unique_ptr<Line>(new Line(x0, y0, x1, y1, graphics, z_order));
  }

private:
  std::unique_ptr<Texture> texture_ = nullptr;
};

#endif /* FRUITCANDY_ENGINE_LINE_H_ */
