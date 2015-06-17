#ifndef GAME_H_
#define GAME_H_

#include "sprite.h"
#include "store.h"
#include "unit.h"
#include "tile.h"
#include "airborne.h"
#include "realm.h"
#include "load_matrix.h"
#include "texture_manager.h"
#include "interaction_manager.h"

class Game: public Realm {
public:
  Game(const unsigned int w, const unsigned int h,
       std::unique_ptr<TextureManager> &texture_manager,
       std::unique_ptr<InteractionManager> &interaction_manager) :
     Realm(w, h),
     texture_manager_(std::move(texture_manager)),
     interaction_manager_(std::move(interaction_manager)) {
    event_hub_.register_listener(
        "mouse_button_down", interaction_manager_.get());
    event_hub_.register_listener(
        "mouse_motion", interaction_manager_.get());
    event_hub_.register_listener(
        "key_down", interaction_manager_.get());
  }
  virtual ~Game() = default;

public:
  void initialize_objects(const std::string &path, int cols, int rows,
                          int grid_width, int grid_height);

  virtual void tick() override;

public:
  std::vector<double> matrix_;
  int cols_ = 0; // max grids at X direction
  int rows_ = 0; // max grids at Y direction
  double grid_width_ = 0; // grid width (pixels)
  double grid_height_ = 0; // grid height (pixels)

public:
  Airborne *airborne_ = nullptr;

public:
  std::unique_ptr<TextureManager> texture_manager_ = nullptr;
  std::unique_ptr<InteractionManager> interaction_manager_ = nullptr;
};

#endif /* GAME_H_ */
