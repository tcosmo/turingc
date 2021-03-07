#pragma once
#include "Turing_machine.h"

static const uint8_t NOTHING_ON_TAPE = 3;

bool tm_parse(TuringMachine* tm, const char* machine_desc_path,
              FILE* machine_desc_file);
bool tm_parse_tape(TuringMachine* tm, const char* tape_content);
