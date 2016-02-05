#ifndef FRUITCANDY_RENDERABLE_TEXT_H_
#define FRUITCANDY_RENDERABLE_TEXT_H_

#include "../core/renderable.h"
#include "../core/font.h"

class Text: public Renderable {
public:
  Text(Graphics *graphics, const std::string &str,
       int font_id, const SDL_Color &color, int z_order = 0)
      : Renderable(graphics, z_order) {
    set_text(graphics, str, font_id, color);
  }
  Text(const Text &text): Renderable(text), texture_(text.texture_) {};
  virtual ~Text() = default;

public:
  virtual void render(const SDL_Rect &rect) const override {
    if (graphics_)
      graphics_->render_texture(texture_.get(), &rect);
    else
      ERROR ("Text: Graphics not found.");
  }

  void set_text(Graphics *graphics, const std::string &str,
                int font_id, const SDL_Color &color) {
    if (graphics_) {
      TTF_SizeText(graphics->assets_->get_item<Font>(font_id)->font_,
                   str.c_str(), &w_, &h_);
      SDL_Surface *surface = TTF_RenderUTF8_Blended(
          graphics->assets_->get_item<Font>(font_id)->font_,
          str.c_str(), color);
      if (!surface)
      	ERROR("Text: Surface for texture not created.");
      SDL_Texture *texture = SDL_CreateTextureFromSurface(
          graphics->renderer_, surface);
      texture_.reset(new Texture(texture));
      SDL_FreeSurface(surface);
      SDL_QueryTexture(texture_->get_texture_ptr(), NULL, NULL, &w_, &h_);
    }
  }

public:
  std::shared_ptr<Texture> texture_;

public:
  // Factory method to create a new instance.
  static std::unique_ptr<Text> create(
      Graphics *graphics, const std::string &str,
      int font_id, const SDL_Color &color, int z_order = 0) {
    return std::unique_ptr<Text>(
        new Text(graphics, str, font_id, color, z_order));
  }
};

#endif /* FRUITCANDY_RENDERABLE_TEXT_H_ */
