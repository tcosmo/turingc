#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define TAPE_SIZE 10000
#define MAX_NB_STATES 1000
#define LINE_BUFFER_SIZE 200

static const uint8_t NOTHING_ON_TAPE = 3;

static const uint8_t FLAG_SET[2] = {1 << 0, 1 << 1};
static const uint8_t FLAG_WRITE[2] = {1 << 2, 1 << 4};
static const uint8_t FLAG_MOVE[2] = {1 << 3, 1 << 5};

typedef struct TuringMachineState TuringMachineState;

typedef struct {
  TuringMachineState* goto_;
  char goto_state_name[LINE_BUFFER_SIZE];
} GotoInstruction;

struct TuringMachineState {
  char state_name[LINE_BUFFER_SIZE];
  uint8_t instr_write_move;
  GotoInstruction instr_goto[2];
};

typedef struct {
  uint8_t tape[TAPE_SIZE];
  size_t head;

  size_t nb_states;

  TuringMachineState* pc;
  TuringMachineState states[MAX_NB_STATES + 1];  // State 0 is always `halt`

  const char* machine_desc_path;
  const char* initial_tape;
} TuringMachine;

bool tm_init(TuringMachine* tm, const char* machine_desc_path,
             const char* initial_tape);
bool tm_next(TuringMachine* tm);
void tm_print_tape(const TuringMachine* tm);

int de_normalize_h(size_t normalised_head_position);
size_t normalize_h(int head_position);