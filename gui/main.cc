#include <memory>

#include "fruitcandy/util/log.h"
#include "fruitcandy/engine/canvas.h"
#include "fruitcandy/engine/viewport.h"

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
               std::unique_ptr<FudgeInteractionManager>(
                 new FudgeInteractionManager(&game))));

  viewport->observe(std::move(camera));

  // Add the viewport to canvas.
  canvas.add_panel(std::move(viewport));

  // Main loop.
  SDL_Event e;
  Uint32 start = SDL_GetTicks(); // Start timer.
  bool running = true;

  while(running) {
    // Handle events.
    while(SDL_PollEvent(&e) != 0) {
      switch (e.type) {
      case SDL_QUIT:
        running = false;
        break;
      default:
        canvas.dispatch_event(e);
      }
    }

    Uint32 now = SDL_GetTicks();
    if (now - start >= 10) { // Timeout. Not likely > 49 days.
      start = now;

      // Tick and render everything.
      game.tick(now);
      canvas.update();
      canvas.render();
    }

    // Pace control.
    SDL_Delay(100);
  }

  return EXIT_SUCCESS;
}
