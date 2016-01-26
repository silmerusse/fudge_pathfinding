#include <memory>

#include "fruitcandy/util/log.h"
#include "fruitcandy/engine/canvas.h"
#include "fruitcandy/engine/viewport.h"
#include "fruitcandy/engine/main_loop.h"

#include "game.h"

int main() {

  // Set map parameters.
  constexpr int kCols = 10;
  constexpr int kRows = 10;
  constexpr int kGridWidth = 32;
  constexpr int kGridHeight = 32;
  constexpr int kWidth = kGridWidth * kCols;
  constexpr int kHeight = kGridHeight * kRows;

  // Create UI.
  Canvas canvas(kWidth, kHeight);
  if (!canvas.init("fudge")) {
    ERROR("Failed to initialize canvas.");
    return EXIT_FAILURE;
  };

  // Load realm.
  Game game(kWidth, kHeight, canvas.get_graphics());
  if (!game.load_assets("../data")) {
    ERROR("Failed to load assets.");
    return EXIT_FAILURE;
  }

  if (!game.initialize_assets()) {
    ERROR("Failed to initialize assets.");
    return EXIT_FAILURE;
  }

  if (!game.initialize_objects("../data", kCols, kRows, kGridWidth, kGridHeight)) {
    ERROR("Failed to initialize objects.");
    return EXIT_FAILURE;
  }

  // Setup a viewport.
  std::unique_ptr<Viewport> viewport(
    new Viewport(canvas.get_graphics(), SDL_Rect{0, 0, kWidth, kHeight}));
  std::unique_ptr<Camera> camera(
    new Camera(SDL_Rect{0, 0, kWidth, kHeight},
               &game,
               {"tiles", "units", "airbornes"}, 
               std::unique_ptr<FudgeEventHandler>(
                 new FudgeEventHandler(&game))));

  viewport->observe(std::move(camera));

  // Add the viewport to canvas.
  canvas.add_panel(std::move(viewport));

  // Main loop.
  main_loop(canvas, [&](unsigned long now){game.tick(now);}, 100);

  return EXIT_SUCCESS;
}
