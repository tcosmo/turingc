#pragma once

#include <SDL2/SDL.h>
#include "Turing_machine.h"

#define TAPE_SQUARE_DIM 100
#define TAPE_REDUNDANCY_SPACE 40

static const uint8_t REDUNDANCY = 2;
static const float CAMERA_ZOOM_FACTOR = 1.5;
static const int CAMERA_Y_STEP = TAPE_SQUARE_DIM;
static const int CAMERA_X_STEP = TAPE_SQUARE_DIM;

typedef enum {
  NORTH = 0,
  EAST,
  SOUTH,
  WEST,
} Direction;

typedef struct {
  const TuringMachine* tm;
  SDL_Renderer* renderer;

  struct {
    float scale;
    int x;
    int y;
  } camera;

} TuringMachineView;

void tm_view_init(TuringMachineView* view, const TuringMachine* tm,
                  SDL_Renderer* renderer);
void tm_view_render(TuringMachineView* view);

void tm_view_camera_zoom(TuringMachineView* view, bool zoom_in);
void tm_view_camera_translate(TuringMachineView* view, Direction dir);