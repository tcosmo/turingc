#include <ctype.h>
#include <string.h>
#include "Turing_machine_parser.h"

TuringMachineState* state_name_already_used_by(TuringMachine* tm,
                                               const char* state_name) {
  for (size_t i_state = 0; i_state < tm->nb_states + 1; i_state += 1) {
    if (strcmp(state_name, tm->states[i_state].state_name) == 0) {
      return (tm->states + i_state);
    }
  }
  return NULL;
}

size_t trim_surrounding_whitespaces(char* out, size_t len, const char* str) {
  if (len == 0) return 0;

  size_t i_char_beg = 0;
  while (i_char_beg < len && isspace(str[i_char_beg])) {
    i_char_beg += 1;
  }

  size_t i_char_end = len - 1;
  while (i_char_end >= 0 && isspace(str[i_char_end])) {
    i_char_end -= 1;
  }

  size_t out_len = i_char_end - i_char_beg + 1;
  memcpy(out, str + i_char_beg, out_len);
  out[out_len] = '\0';
  return out_len;
}

bool is_empty(const char* str) {
  for (size_t i = 0; i < strlen(str); i += 1)
    if (!isspace(str[i])) return false;
  return true;
}

bool tm_parse(TuringMachine* tm, const char* machine_desc_path,
              FILE* machine_desc_file) {
  memcpy(tm->states[0].state_name, "halt", LINE_BUFFER_SIZE);
  tm->states[0].instr_write_move = 0;  // Halting state
  tm->pc = tm->states + 1;

  char buffer[LINE_BUFFER_SIZE];

  size_t i_line = 0;
  size_t i_state = 0;
  size_t true_line = 0;

  while (fgets(buffer, LINE_BUFFER_SIZE, machine_desc_file)) {
    true_line += 1;

    if (buffer[0] == ';' || is_empty(buffer)) continue;

    // State line
    if (i_line % 3 == 0) {
      if (!strstr(buffer, ":")) {
        printf(
            "Formatting error in `%s` at line %zu:\n\t `%s`\n A state "
            "line "
            "should "
            "contain `:`. Abort.\n",
            machine_desc_path, true_line, buffer);
        return false;
      }

      char* state_name = strtok(buffer, ":");
      char trimmed_state_name[LINE_BUFFER_SIZE];

      trim_surrounding_whitespaces(trimmed_state_name, strlen(state_name),
                                   state_name);

      if (state_name_already_used_by(tm, trimmed_state_name)) {
        printf(
            "Formatting error in `%s` at line %zu, state name `%s` is already "
            "used (`halt` is always reserved). Abort.\n",
            machine_desc_path, true_line, trimmed_state_name);
        return false;
      }

      tm->nb_states += 1;

      if (tm->nb_states >= MAX_NB_STATES) {
        printf(
            "Formatting error in `%s` at line %zu, too many states are "
            "described, maximum "
            "is: %d\n",
            machine_desc_path, true_line, MAX_NB_STATES);
        return false;
      }

      memcpy(tm->states[tm->nb_states].state_name, trimmed_state_name,
             LINE_BUFFER_SIZE);
    }

    // Read 0, read 1
    if (i_line % 3 > 0) {
      TuringMachineState* current_state = tm->states + tm->nb_states;
      bool read_1 = (i_line % 3) == 2;

      if (!strstr(buffer, ",")) {
        current_state->instr_write_move &= ~FLAG_SET[read_1];
        i_line += 1;
        continue;
      }

      char* comma_tok = strtok(buffer, ",");
      uint8_t tok_count = 0;
      while (comma_tok != NULL) {
        tok_count += 1;

        char trimmed_tok[LINE_BUFFER_SIZE];
        trim_surrounding_whitespaces(trimmed_tok, strlen(comma_tok), comma_tok);

        // Write instruction
        if (tok_count == 1) {
          current_state->instr_write_move |= FLAG_SET[read_1];

          if (strcmp(trimmed_tok, "0") == 0) {
            current_state->instr_write_move &= ~FLAG_WRITE[read_1];
          } else if (strcmp(trimmed_tok, "1") == 0) {
            current_state->instr_write_move |= FLAG_WRITE[read_1];
          } else {
            printf(
                "Formatting error in `%s` at line %zu, invalid "
                "write instruction:\n\t %s\n Abort.",
                machine_desc_path, true_line, trimmed_tok);
            return false;
          }
        }

        // Read instruction
        if (tok_count == 2) {
          if (strcmp(trimmed_tok, "L") == 0) {
            current_state->instr_write_move &= ~FLAG_MOVE[read_1];
          } else if (strcmp(trimmed_tok, "R") == 0) {
            current_state->instr_write_move |= FLAG_MOVE[read_1];
          } else {
            printf(
                "Formatting error in `%s` at line %zu, invalid "
                "move instruction:\n\t %s\n Abort.",
                machine_desc_path, true_line, trimmed_tok);
            return false;
          }
        }

        // Goto instruction
        if (tok_count == 3) {
          memcpy(current_state->instr_goto[read_1].goto_state_name, trimmed_tok,
                 LINE_BUFFER_SIZE);
        }

        comma_tok = strtok(NULL, ",");
      }

      if (tok_count > 0 && tok_count < 3) {
        printf(
            "Formatting error in `%s` at line %zu, instruction "
            "is partial, not allowed (just put `-` instead of a partial "
            "instruction). Abort.",
            machine_desc_path, true_line);
        return false;
      }
    }

    i_line += 1;
  }

  for (size_t i_state = 1; i_state <= tm->nb_states; i_state += 1) {
    for (size_t read_1 = 0; read_1 <= 1; read_1 += 1) {
      if (!(tm->states[i_state].instr_write_move & FLAG_SET[read_1])) continue;

      TuringMachineState* goto_points_to = state_name_already_used_by(
          tm, tm->states[i_state].instr_goto[read_1].goto_state_name);

      if (!goto_points_to) {
        // Special case for 'H' shortcut for halts
        if (strcmp(tm->states[i_state].instr_goto[read_1].goto_state_name,
                   "H") == 0) {
          memcpy(tm->states[i_state].instr_goto[read_1].goto_state_name, "halt",
                 LINE_BUFFER_SIZE);
          tm->states[i_state].instr_goto[read_1].goto_ = tm->states;
        } else {
          printf(
              "Formatting error in `%s`: state `%s` is referred to in goto "
              "instructions but"
              "does not exists.Abort.\n ",
              machine_desc_path,
              tm->states[i_state].instr_goto[read_1].goto_state_name);
          return false;
        }
      } else {
        tm->states[i_state].instr_goto[read_1].goto_ = goto_points_to;
      }
    }
  }

  return true;
}

bool tm_parse_tape(TuringMachine* tm, const char* tape_content) {
  // printf("Parsing tape %s\n", tape_content);

  memset(tm->tape, NOTHING_ON_TAPE, TAPE_SIZE);

  size_t tape_size = strlen(tape_content) - 2;
  if (tape_size >= TAPE_SIZE) {
    printf("Tape input of size %zu is too big for tape size %d. Abort.\n",
           tape_size, TAPE_SIZE);
    return false;
  }

  int pos_on_tape = 0;

  for (size_t i_char = 0; i_char < strlen(tape_content); i_char += 1) {
    switch (tape_content[i_char]) {
      case '0':
        tm->tape[normalize_h(pos_on_tape)] = 0;
        pos_on_tape += 1;
        break;
      case '1':
        tm->tape[normalize_h(pos_on_tape)] = 1;
        pos_on_tape += 1;
        break;
      case '>':
        tm->head = normalize_h(pos_on_tape);
        break;
      case ' ':
        continue;
      default:
        printf("Unexpected character on tape: `%c`. Abort.\n",
               tape_content[i_char]);
        return false;
        break;
    }
  }

  return true;
}