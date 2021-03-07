#include "simulator.h"

const char* TURING_MACHINE_DESC_PATH =
    "assets/machines/parity_with_redundancy.txt";

int main(int argc, char** argv) {
  TuringMachine tm;

  if (!tm_init(&tm, TURING_MACHINE_DESC_PATH, (argc > 1) ? argv[1] : "")) {
    return 1;
  }
  tm_print_tape(&tm);

  int i = 0;
  while (tm_next(&tm)) {
    tm_print_tape(&tm);
    // i += 1;
    // if (i == 3) break;
  }

  return 0;
  // return run_simulator(&tm);
}