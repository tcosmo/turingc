#include "Turing_machine_view.h"

void tm_view_init(TuringMachineView* view, const TuringMachine* tm,
                  SDL_Renderer* renderer) {
  view->tm = tm;
  view->renderer = renderer;
  view->camera.scale = 1.0;
  view->camera.x = tm->head * TAPE_SQUARE_DIM +
                   (tm->head * TAPE_REDUNDANCY_SPACE) / REDUNDANCY;
  view->camera.y = 200;

  view->camera.scale = 0.296296;
  SDL_RenderSetScale(view->renderer, view->camera.scale, view->camera.scale);
  view->camera.x = 598800;
  view->camera.y = 1000;
}

void tm_view_render(TuringMachineView* view) {
  SDL_Rect square_rect = {-1 * view->camera.x, view->camera.y, TAPE_SQUARE_DIM,
                          TAPE_SQUARE_DIM};

  for (size_t i_square = 0; i_square < TAPE_SIZE; i_square += 1) {
    if (view->tm->tape[i_square] != NOTHING_ON_TAPE) {
      SDL_SetRenderDrawColor(view->renderer,
                             (1 - view->tm->tape[i_square]) * 255,
                             (1 - view->tm->tape[i_square]) * 255,
                             (1 - view->tm->tape[i_square]) * 255, 200);
      SDL_RenderFillRect(view->renderer, &square_rect);

      SDL_SetRenderDrawColor(view->renderer, 255, 255, 255, 255);
      SDL_RenderDrawRect(view->renderer, &square_rect);
    }

    if (i_square == view->tm->head) {
      SDL_SetRenderDrawColor(view->renderer, 200, 20, 200, 150);
      SDL_RenderFillRect(view->renderer, &square_rect);
    }

    square_rect.x += TAPE_SQUARE_DIM;

    if (abs(de_normalize_h(i_square)) % REDUNDANCY == (REDUNDANCY - 1)) {
      square_rect.x += TAPE_REDUNDANCY_SPACE;
    }
  }
}

void tm_view_camera_zoom(TuringMachineView* view, bool zoom_in) {
  view->camera.scale *= (zoom_in) ? CAMERA_ZOOM_FACTOR : 1 / CAMERA_ZOOM_FACTOR;
  SDL_RenderSetScale(view->renderer, view->camera.scale, view->camera.scale);
}

void tm_view_camera_translate(TuringMachineView* view, Direction dir) {
  switch (dir) {
    case NORTH:
      view->camera.y += CAMERA_Y_STEP;
      break;
    case EAST:
      view->camera.x += CAMERA_X_STEP;
      break;
    case SOUTH:
      view->camera.y -= CAMERA_Y_STEP;
      break;
    case WEST:
      view->camera.x -= CAMERA_X_STEP;
      break;
    default:
      break;
  }
}