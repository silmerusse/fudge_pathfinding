#ifndef FRUITCANDY_CORE_FONT_H_
#define FRUITCANDY_CORE_FONT_H_

#include <SDL2/SDL_ttf.h>

#include "../util/log.h"

#include "asset.h"

class Font: public Asset {
public:
  Font() {
    loader_ = FontLoader::instance();
  }
  virtual ~Font() {
    if (font_) {
      TTF_CloseFont(font_);
    }
  }

  bool open() {
    font_ = TTF_OpenFont(path_.c_str(), size_);
    if (font_) {
      TTF_SetFontHinting(font_, TTF_HINTING_LIGHT);
      DEBUG("TTF font initialized: %s", name_.c_str());
    } else {
      DEBUG("Unable to initialize TTF font.");
      return false;
    }
    return true;
  }

public:
  int size_ = 16;
  TTF_Font *font_ = nullptr;

private:
  class FontLoader : public Loader {
    public:
    FontLoader() = default;
    virtual ~FontLoader() = default;

  public:
    virtual void load(Item *item, Value &v) override {
      Font *font = static_cast<Font*>(item);
      font->name_ = v.get_string("name");
      font->path_ = v.get_string("path");
      font->size_ = v.get_int("size");
    }

    static FontLoader* instance() {
      static FontLoader instance_;
      return &instance_;
    }
  };
};

#endif /* FRUITCANDY_CORE_FONT_H_ */
