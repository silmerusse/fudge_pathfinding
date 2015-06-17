#include <memory>
#include "log.h"
#include "canvas.h"
#include "game.h"
#include "viewport.h"

int main() {

  // Set map parameters.
  constexpr int kCols = 10;
  constexpr int kRows = 10;
  constexpr int kGridWidth = 32;
  constexpr int kGridHeight = 32;
  constexpr int kWidth = kGridWidth * kCols;
  constexpr int kHeight = kGridHeight * kRows;

  // Create UI.
  Canvas canvas;
  if (!canvas.init("fudge", kWidth, kHeight)) {
    ERROR("Failed to initialize canvas.");
    return -1;
  };

  // Load textures.
  std::unique_ptr<TextureManager> texture_manager(new TextureManager());
  if (!texture_manager->load_textures(canvas.renderer_,
                        {{"tile", "img/tile.png"},
                         {"wall", "img/wall.png"},
                         {"unit", "img/unit.png"}})) {
    ERROR("Failed to load textures.");
    return -1;
  }

  std::unique_ptr<InteractionManager>
  interaction_manager(new InteractionManager());

  // Load game.
  Game game(kWidth, kHeight, texture_manager, interaction_manager);
  game.initialize_objects("data", kCols, kRows, kGridWidth, kGridHeight);

  // Setup a viewport.
  std::unique_ptr<Viewport> v(new Viewport(SDL_Rect{0, 0, kWidth, kHeight}));
  std::unique_ptr<Camera> camera(new Camera(SDL_Rect{0, 0, kWidth, kHeight},
                                            &game,
                                            {"tiles", "units", "airbornes"}));
  v->observe(std::move(camera));

  // Add the viewport to canvas.
  canvas.add_panel(std::move(v));

  // Main loop.
  SDL_Event e;
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

    // Tick and render everything.
    game.tick();
    canvas.render();

    // Pace control.
    SDL_Delay(100);
  }

  return 0;
}
