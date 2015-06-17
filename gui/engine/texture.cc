#include "texture.h"

bool Texture::load(SDL_Renderer *renderer, const std::string &filename) {
  texture_ = IMG_LoadTexture(renderer, filename.c_str());
  return texture_ != nullptr;
}
