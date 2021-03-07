#include "simulator.h"

const char* TURING_MACHINE_DESC_PATH = "assets/machines/Collatz.txt";

int main(int argc, char** argv) {
  TuringMachine tm;

  if (!tm_init(&tm, TURING_MACHINE_DESC_PATH, (argc > 1) ? argv[1] : "")) {
    return 1;
  }
  // tm_print_tape(&tm);

  int nb_steps = 0;

  return run_simulator(&tm, nb_steps);
}