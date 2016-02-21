#ifndef FRUITCANDY_ENGINE_SCENE_MANAGER_H_
#define FRUITCANDY_ENGINE_SCENE_MANAGER_H_

#include <map>
#include <string>

#include "scene.h"

class SceneManager {
public:
  SceneManager() = default;
  virtual ~SceneManager() = default;

public:
  void register_scene(const std::string name, Scene *scene) {
    scenes_[name] = scene;
    scene->manager_ = this;
    scene->enabled_ = false; // Disable scene by default.
  }

  Scene* get_scene(const std::string name) {
    if (scenes_.find(name) != scenes_.end()) {
      return scenes_.at(name);
    } else {
      ERROR ("Scene not found: %s", name.c_str());
      return nullptr;
    }
  }

  void switch_to (const std::string name) {
    if (current_)
      current_->enabled_ = false;

    current_ = get_scene(name);

    if (current_)
      current_->enabled_ = true;
  }

  void tick(Uint32 ticks) {
    if (current_)
      current_->tick(ticks);
  }

public:
  std::map<std::string, Scene*> scenes_;
  Scene *current_ = nullptr;
};

#endif /* FRUITCANDY_ENGINE_SCENE_SCENE_MANAGER_H_ */
