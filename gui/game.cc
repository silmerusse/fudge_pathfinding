#include "object/tile.h"
#include "object/unit.h"
#include "fruitcandy/json/json_item_loader.h"
#include "game.h"
#include "load_matrix.h"

bool Game::load_assets(const std::string &path) {
  JSONItemLoader loader;
  if (!loader.load_items<Texture>(graphics_->assets_.get(), 
                                  "texture",
                                  path + "/textures_config.json")) {
    ERROR("Unable to load textures.");
    return false;
  }
  INFO("Assets loaded.");
  return true;
}

bool Game::initialize_assets() {
  graphics_->assets_->traverse<Texture>("texture",
                                        std::bind([&](Texture *texture) {
    texture->open(graphics_->renderer_);
  }, std::placeholders::_1));
  INFO("Textures initialized.");

  INFO("Assets initialized.");
  return true;
}

bool Game::initialize_objects(const std::string &path, int cols, int rows,
                              int grid_width, int grid_height) {
  cols_ = cols;
  rows_ = rows;
  grid_width_ = grid_width;
  grid_height_ = grid_height;

  matrix_ = load_matrix<double>(path + "/matrix_10x10_wall.txt");

  // Create tiles.
  for (auto r = 0; r < rows; ++r) {
    for (auto c = 0; c < cols; ++c) {
      Tile *tile = repo_->create_item<Tile>("tiles");
      tile->transform_->set_position({c * 32 + 16.0, r * 32 + 16.0});
      tile->pos_ = {c, r};

      if (matrix_[tile->pos_.first + tile->pos_.second * 10] != -1) {
        tile->add_child(std::move(
            Sprite::create(graphics_, 32, 32, TEXTURE_TILE, nullptr, 10)));
      } else {
        tile->add_child(std::move(
            Sprite::create(graphics_, 32, 32, TEXTURE_WALL, nullptr, 10)));
      }
    }
  }

  // Create units.
  Unit *unit = repo_->create_item<Unit>("units");
  unit->game_ = this;
  unit->move_to({0, 0});
  unit->add_child(std::move(
      Sprite::create(graphics_, 32, 32, TEXTURE_UNIT, nullptr, 10)));

  // Creat airborne.
  airborne_ = repo_->create_item<Airborne>("airbornes");
  airborne_->game_ = this;
  airborne_->add_child(std::move(
      Sprite::create(graphics_, 32, 32, TEXTURE_UNIT, nullptr, 10)));
  airborne_->sprite_visible(false);

  INFO("Object initialized.");
  return true;
}

