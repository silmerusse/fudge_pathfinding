#include "log.h"
#include "texture_manager.h"

bool TextureManager::load_texture(SDL_Renderer *renderer,
                                  const std::string &name,
                                  const std::string &path) {
  textures_[name] = std::make_shared<Texture>();
  if (textures_[name]->load(renderer, path)) {
    DEBUG("Texture loaded: %s", path.c_str());
    return true;
  } else {
    ERROR("Load texture failed: %s", path.c_str());
    return false;
  }
}

bool TextureManager::load_textures(
    SDL_Renderer *renderer,
    const std::vector<std::pair<const std::string, const std::string>> &configs) {
  for (auto c : configs) {
    if (!load_texture(renderer, c.first, c.second))
      return false;
  }
  return true;
}

const std::shared_ptr<Texture> TextureManager::get_texture(
    const std::string &name) const {
  return textures_.at(name);
}
