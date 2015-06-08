#ifndef GAME_H_
#define GAME_H_

#include "sprite.h"
#include "store.h"
#include "texture_manager.h"
#include "unit.h"
#include "tile.h"
#include "airborne.h"
#include "realm.h"
#include "load_matrix.h"

class Game: public Realm {
public:
  Game(const unsigned int w, const unsigned int h,
       std::unique_ptr<TextureManager> &texture_manager) :
      Realm(w, h, texture_manager) {
  }
  virtual ~Game() = default;

public:
  void initialize_objects(const std::string &path, int cols, int rows,
                          int grid_width, int grid_height) {
    cols_ = cols;
    rows_ = rows;
    grid_width_ = grid_width;
    grid_height_ = grid_height;

    matrix_ = load_matrix<double>("../data/matrix_10x10_wall.txt");

    // Create tiles.

    object_manager_->add_store("tiles");
    for (auto r = 0; r < rows; ++r) {
      for (auto c = 0; c < cols; ++c) {
        Tile *tile = object_manager_->create_object<Tile>("tiles");
        tile->set_position({c * 32 + 16.0, r * 32 + 16.0});
        tile->pos_ = {c, r};

        if (matrix_[tile->pos_.first + tile->pos_.second * 10] != -1) {
          tile->add_component(std::move(
              Sprite::create(texture_manager_->get_texture("tile").get(), 10)));
        } else {
          tile->add_component(std::move(
              Sprite::create(texture_manager_->get_texture("wall").get(), 10)));
        }
      }
    }

    // Create units.
    object_manager_->add_store("units");
    Unit *unit = object_manager_->create_object<Unit>("units");
    unit->move_to(this, {0, 0});

    unit->add_component(std::move(
        Sprite::create(texture_manager_->get_texture("unit").get(), 10)));

    // Register listener so that they could know if being selected.
    event_queue_.register_listener("mouse_button_down", unit);

    // Creat airborne.
    object_manager_->add_store("airbornes");
    Airborne *airborne = object_manager_->create_object<Airborne>("airbornes");
    airborne->add_component(std::move(
        Sprite::create(texture_manager_->get_texture("unit").get(), 10)));
    airborne->sprite_visible(false);

    // Register listener so that they could know if where to render.
    event_queue_.register_listener("mouse_motion", airborne);

    // Register listener so that they could know if when show up.
    event_queue_.register_listener("key_down", airborne);

    // Register listener so that they could know if when to drop.
    event_queue_.register_listener("mouse_button_down", airborne);

    INFO("Game loaded.");
  }

public:
  std::vector<double> matrix_;
  int cols_ = 0; // max grids at X direction
  int rows_ = 0; // max grids at Y direction
  double grid_width_ = 0; // grid width (pixels)
  double grid_height_ = 0; // grid height (pixels)
};

#endif /* GAME_H_ */
