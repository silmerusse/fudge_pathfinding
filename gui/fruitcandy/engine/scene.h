#ifndef FRUITCANDY_ENGINE_SCENE_H_
#define FRUITCANDY_ENGINE_SCENE_H_

#include "panel.h"

class SceneManager;

class Scene : public Panel {
public:
  Scene(Graphics *graphics, int w, int h, int z_order = 0)
      : Panel(graphics, {0, 0, w, h}, z_order) {};
  virtual ~Scene() = default;

public:
  virtual void tick(unsigned long ticks) {
    if (handler_)
      handler_(ticks);
  }

  virtual void set_tick_handler(std::function<void(Uint32)> handler) {
    handler_ = handler;
  }

public:
  SceneManager *manager_ = nullptr;
  std::function<void(Uint32)> handler_;
};

#endif /* FRUITCANDY_ENGINE_SCENE_H_ */
