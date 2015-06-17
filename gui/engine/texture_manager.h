#ifndef TEXTURE_MANAGER_H_
#define TEXTURE_MANAGER_H_

#include <string>
#include <map>
#include <vector>
#include <memory>
#include "texture.h"
#include "module.h"

class TextureManager : public Module {
public:
  TextureManager() = default;
  virtual ~TextureManager() = default;

public:
  bool load_textures(
      SDL_Renderer *renderer,
      const std::vector<std::pair<const std::string, const std::string>>
          &configs);
  const std::shared_ptr<Texture> get_texture(const std::string &name) const;

private:
  bool load_texture(SDL_Renderer *renderer, const std::string &name,
                    const std::string &path);

private:
  std::map<std::string, std::shared_ptr<Texture>> textures_;
};

#endif /* TEXTURE_MANAGER_H_ */
