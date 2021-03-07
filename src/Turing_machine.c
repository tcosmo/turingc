#include "Turing_machine.h"
#include "Turing_machine_parser.h"
// Switch from possibly negative head position to head position in [0,
// MAX_TAPE_SIZE]
size_t normalize_h(int head_position) {
  if (head_position >= TAPE_SIZE / 2) {
    printf("Tape head going to far right (> %d)\n", TAPE_SIZE / 2);
    exit(0);
  }

  if (head_position < -1 * TAPE_SIZE / 2) {
    printf("Tape head going to far left (< %d)\n", -1 * TAPE_SIZE / 2);
    exit(0);
  }

  return head_position + TAPE_SIZE / 2;
}

int de_normalize_h(size_t normalised_head_position) {
  return normalised_head_position - TAPE_SIZE / 2;
}

void tm_print_tape(const TuringMachine* tm) {
  for (size_t i = 0; i < TAPE_SIZE; i += 1) {
    if (tm->head == i) {
      printf(">");
      if (tm->tape[i] == NOTHING_ON_TAPE) {
        printf("0");
      }
    }
    if (tm->tape[i] != NOTHING_ON_TAPE) {
      printf("%c", '0' + tm->tape[i]);
    }
  }
  printf("\n");
}

bool tm_init(TuringMachine* tm, const char* machine_desc_path,
             const char* initial_tape) {
  FILE* machine_desc_file = fopen(machine_desc_path, "r");
  if (machine_desc_file == NULL) {
    printf("Cannot open machine file `%s`.\n", machine_desc_path);
    return false;
  }

  if ((!tm_parse(tm, machine_desc_path, machine_desc_file)) ||
      (!tm_parse_tape(tm, initial_tape))) {
    return false;
  }

  fclose(machine_desc_file);

  return true;
}

bool tm_next(TuringMachine* tm) {
  assert(!tm->head < MAX_NB_STATES);

  TuringMachineState* current_state = tm->pc;

  if (!current_state->instr_write_move) {
    printf("Machine halted in state `%s`\n", current_state->state_name);
    return false;
  }

  bool read_1 = tm->tape[tm->head] == 1;

  if (!(current_state->instr_write_move & FLAG_SET[read_1])) {
    printf("Machine halted in state `%s`\n", current_state->state_name);
    return false;
  }

  tm->tape[tm->head] =
      (current_state->instr_write_move & FLAG_WRITE[read_1]) != 0;

  tm->head = normalize_h(
      de_normalize_h(tm->head) +
      ((current_state->instr_write_move & FLAG_MOVE[read_1]) ? 1 : -1));

  tm->pc = current_state->instr_goto[read_1].goto_;
}