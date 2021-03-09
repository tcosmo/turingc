#include <string.h>
#include "simulator.h"
const char* TURING_MACHINE_DESC_PATH = "assets/machines/CollatzSecondTry.txt";

int main(int argc, char** argv) {
  TuringMachine tm;

  if (!tm_init(&tm, TURING_MACHINE_DESC_PATH, (argc > 1) ? argv[1] : "")) {
    return 1;
  }
  // tm_print_tape(&tm);

  int nb_steps = 0;

  // while (strcmp(tm.pc->state_name, "Collatz_start_ignore_trailing_zeros") !=
  //        0) {
  //   tm_next(&tm);
  //   nb_steps += 1;
  // }
  return run_simulator(&tm, nb_steps);
}