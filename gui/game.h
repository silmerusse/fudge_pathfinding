#ifndef FUDGE_PATHFINDING_GUI_GAME_H_
#define FUDGE_PATHFINDING_GUI_GAME_H_

#include <fruitcandy/renderable/sprite.h>
#include <fruitcandy/core/realm.h>

#include "object/airborne.h"
#include "fudge_event_handler.h"

#define TEXTURE_TILE 1
#define TEXTURE_WALL 2
#define TEXTURE_UNIT 3

class Game: public Realm {
public:
  Game(const unsigned int w, const unsigned int h, Graphics *graphics)
    :Realm(w, h), graphics_(graphics) {}
  virtual ~Game() = default;

public:
  bool load_assets(const std::string &path) override;
  bool initialize_assets() override;
  bool initialize_objects(const std::string &path, int cols, int rows,
                          int grid_width, int grid_height);

public:
  std::vector<double> matrix_;
  int cols_ = 0; // max grids at X direction
  int rows_ = 0; // max grids at Y direction
  double grid_width_ = 0; // grid width (pixels)
  double grid_height_ = 0; // grid height (pixels)

public:
  Airborne *airborne_ = nullptr;

public:
  Graphics *graphics_ = nullptr;
};

#endif /* FUDGE_PATHFINDING_GUI_GAME_H_ */
