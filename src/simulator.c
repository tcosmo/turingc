#include <SDL2/SDL.h>
#include "Turing_machine_view.h"
#include "simulator.h"

int run_simulator(TuringMachine* tm, int nb_steps) {
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

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  TuringMachineView view;
  tm_view_init(&view, tm, renderer);
  printf("Current state: `%s`\n", tm->pc->state_name);
  bool simulation_running = true;

  for (int i_step = 0; i_step < nb_steps; i_step += 1) tm_next(tm);

  while (simulation_running) {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) simulation_running = false;

      if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          case SDLK_i:
            printf("=== Machine Infos ===\n");
            printf("\tnb states: %zu\n", tm->nb_states);
            printf("\tnb simulation steps: %d\n", nb_steps);
            printf("=== Camera Settings ===\n");
            printf("\tscale: %f\n", view.camera.scale);
            printf("\tx: %d\n", view.camera.x);
            printf("\ty: %d\n", view.camera.y);
            break;

          case SDLK_ESCAPE:
            simulation_running = false;
            break;

          case SDLK_n:
            if (tm_next(tm)) {
              nb_steps += 1;
              printf("Current state: `%s`\n", tm->pc->state_name);
            }
            break;

          case SDLK_z:
            tm_view_camera_zoom(&view, true);
            break;
          case SDLK_a:
            tm_view_camera_zoom(&view, false);
            break;
          case SDLK_UP:
            tm_view_camera_translate(&view, NORTH);
            break;
          case SDLK_RIGHT:
            tm_view_camera_translate(&view, EAST);
            break;
          case SDLK_DOWN:
            tm_view_camera_translate(&view, SOUTH);
            break;
          case SDLK_LEFT:
            tm_view_camera_translate(&view, WEST);
            break;
        }
      }
    }

    SDL_SetRenderDrawColor(renderer, 100, 149, 237, 255);
    SDL_RenderClear(renderer);

    tm_view_render(&view);

    SDL_RenderPresent(renderer);
  }

  // tm_view_clean(&view);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}