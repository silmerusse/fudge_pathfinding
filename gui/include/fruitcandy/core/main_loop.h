#ifndef FRUITCANDY_CORE_MAINLOOP_H_
#define FRUITCANDY_CORE_MAINLOOP_H_

#include <functional>
#include "canvas.h"
#include "scene_manager.h"

void main_loop(Canvas &canvas, 
               std::function<void(unsigned long)>tick, 
               Uint32 delay=0) {
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
      tick(now);
      canvas.update();
      canvas.render();
    }
    // Pace control.
    SDL_Delay(delay);
  }
}
#endif /* FRUITCANDY_CORE_MAINLOOP_H_ */
