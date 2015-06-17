#include "game.h"

void Game::initialize_objects(const std::string &path, int cols, int rows,
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

  // Creat airborne.
  object_manager_->add_store("airbornes");
  airborne_ = object_manager_->create_object<Airborne>("airbornes");
  airborne_->add_component(std::move(
      Sprite::create(texture_manager_->get_texture("unit").get(), 10)));
  airborne_->sprite_visible(false);

  INFO("Game loaded.");
}

void Game::tick() {
  interaction_manager_->handle_events(this);
  Realm::tick();
}
