#include <SDL2/SDL.h>
#include "simulator.h"

int run_simulator(TuringMachine* tm) {
  SDL_Window* window;

  SDL_Init(SDL_INIT_VIDEO);

  window =
      SDL_CreateWindow("Turing Machine", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 960, 740, SDL_WINDOW_OPENGL);

  if (window == NULL) {
    printf("Could not create window: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Renderer* renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    printf("Could not create renderer: %s\n", SDL_GetError());
    return 1;
  }

  bool simulation_running = true;
  while (simulation_running) {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) simulation_running = false;

      if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          case SDLK_ESCAPE:
            simulation_running = false;
            break;
        }
      }
    }

    SDL_SetRenderDrawColor(renderer, 100, 149, 237, 255);
    SDL_RenderClear(renderer);

    // tm_view_render(renderer, &view);

    SDL_RenderPresent(renderer);
  }

  // tm_view_clean(&view);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}