/**
 * @file shingeta.c
 * @brief 新下駄入力の実装
 * @author lum1narie
 * @date 2018/06/02
 */
// call init_shingeta_mode() at first of all
// call on_enter_shingeta_mode() when enter the shingeta mode and
// call on_exit_shingeta_mode() when exit the shingeta mode
#include QMK_KEYBOARD_H
#include "shingeta.h"

#include <avr/pgmspace.h>
#include <quantum.h>
#include <stdbool.h>
#include <stdio.h>

#include "ergodox_ez.h"

#ifdef SHINGETA_DEBUG_ENABLE
#include "print.h"
#endif

// keep last inputed keys
shingeta_record_t last_sgta_record;

volatile xtu_mode_t xtu_mode = NO_XTU;
volatile bool is_xtu_shifted = false;

#ifdef SHINGETA_DEBUG_ENABLE
/**
 * @brief print the detail of shingeta record to debug console
 *
 * @param shingeta_record record to print
 */
void print_shingeta_record(shingeta_record_t *shingeta_record) {
  // xprintf("record\n");
  // xprintf("adress: %08lX\n", shingeta_record->record);
  xprintf("record.event.key.row: %X\n", shingeta_record->record.event.key.row);
  xprintf("record.event.key.col: %X\n", shingeta_record->record.event.key.col);
  xprintf("is_shifted: %X\nis_alive: %X\n", shingeta_record->is_shifted,
          shingeta_record->is_alive);
  xprintf("------------------------------\n");
  return;
}
#endif

// define functions only for this file
// ==============================================================
// from here

/**
 * @brief reset flags in shingeta_record_t
 *
 * make is_shifted and is_alive false in record
 * @param shingeta_record record to reset
 */
inline void reset_shingeta_record(shingeta_record_t *shingeta_record) {
  shingeta_record->is_shifted = false;
  shingeta_record->is_alive = false;
  return;
}

/**
 * @brief convert keyrecord in shingeta_record_t into one 8bit integer
 *
 * compress into one 8bit number so as make it easy to be decoded in string
 *
 * @param shingeta_record record to convert
 * @return code that is 8bit number
 */
inline uint8_t get_code_from_shingeta_record(
    shingeta_record_t *shingeta_record) {
  return position_to_8bit(shingeta_record->record.event.key);
}

/**
 * @brief get string match with (code1, code2), or (code1, -1) if not found
 *
 * @param code1 former code, also used in second try when mismatch
 * @param code2 latter code, treated as -1 in second try when mismatch
 * @param ret_str pointer of string_
 *
 * @retval 0 a string matched with (code1, code2)
 * @retval 1 no string matched with (code1, code2), but one matched with (code1,
 * CODE_NA)
 * @retval 2 no string matched with both of above are not found, ERROR (not
 * should be occured)
 *
 * found string will be assigned in ret_str
 */
int search_string_by_code(uint8_t code1, uint8_t code2, char *ret_str) {
  // take string match with (code1, code2)
  get_str_from_code(code1, code2, ret_str);

#ifdef SHINGETA_DEBUG_ENABLE
  xprintf("searching string by code\n");
  xprintf("code1: 0x%X, code2: 0x%X\n", (unsigned int)code1,
          (unsigned int)code2);
  xprintf("%s\n", ret_str);
  xprintf("----------------------------------------");
#endif

  // if string matching (code1, code2) is found
  if (strcmp(ret_str, "") != 0) {
    return 0;
  }

  // if string matching (code1, -1) is NOT found
  // then if string matching (code1, -1) is found
  get_str_from_code(code1, -1, ret_str);
#ifdef SHINGETA_DEBUG_ENABLE
  xprintf("searching again\n");
  xprintf("%s \n", ret_str);
  xprintf("----------------------------------------");
#endif
  if (strcmp(ret_str, "") != 0) {
    return -1;
  }

  // if no string found
  // ERROR -- this should not be happen
#ifdef SHINGETA_CODE_STRICT
  SEND_STRING(ERROR_MESSAGE_MISMATCH_SHINGETA_CODE);
#endif
  return 2;
}

/**
 * @brief common procedure of string sending commands
 * this deal with xtu_mode
 *
 * @param string: given string required to be prefixed by xtu
 * give "" if no need to prefix
 *
 * @retval prefixed string by xtu
 */

char *xtu_common(char string[]) {
  if (xtu_mode == CONTINUOUS_XTU) {
    xtu_mode = NO_XTU;
  }

  // prefix string with xtu if need
  if (xtu_mode == SINGLE_XTU) {
    xtu_letter(string);
    if (is_xtu_shifted) {
      shift_capital_letter(string);
    }

    xtu_mode = NO_XTU;
  }

  return string;
}

int jp_into_brackets(char *string, bool is_shifted) {
  string[0] = '\0';
  xtu_common(string);
  send_string(string);
  SEND_STRING("]\\" SS_TAP(X_LEFT));  // "[]"<- in JP
  return 0;
}
int jp_into_parentheses(char *string, bool is_shifted) {
  string[0] = '\0';
  xtu_common(string);
  send_string(string);
  SEND_STRING("*(" SS_TAP(X_LEFT));  // "()"<- in JP
  return 0;
}
int jp_backslash_input(char *string, bool is_shifted) {
  string[0] = '\0';
  xtu_common(string);
  send_string(string);
  SEND_STRING(SS_TAP(X_INT1));  // "/" in JP
  return 0;
}
int xtu_input(char *string, bool is_shifted) {
  switch (xtu_mode) {
    case NO_XTU:
      xtu_mode = SINGLE_XTU;
      is_xtu_shifted = is_shifted;
      break;
    case SINGLE_XTU:
      xtu_mode = CONTINUOUS_XTU;
    case CONTINUOUS_XTU:
      SEND_STRING("xtu");
      break;
    default:
      break;
  }
  return 0;
}
int shingeta_pre_suffix_input(char *string, bool is_shifted) {
  string[0] = '\0';
  xtu_common(string);
  send_string(string);
  if (is_shifted) {
    SEND_STRING("Q>");
  } else {
    SEND_STRING(">");
  }
  return 0;
}

int (*functions_for_command[])(char *, bool) = {
    jp_into_brackets, jp_into_parentheses, jp_backslash_input, xtu_input,
    shingeta_pre_suffix_input};

/**
 * @breif send ordinal string in shingeta
 * check if xtu is enable, or shifted
 */
int send_normal_string(char string[], bool is_shifted) {
  if (is_shifted) {
    shift_capital_letter(string);
  }
  xtu_common(string);
  send_string(string);

  return 0;
}

/**
 * @brief send string or do command that given codes refers
 *
 * @param code1 former code, also used in second try when mismatch
 * @param code2 latter code, treated as -1 in second try when mismatch
 * @param is_shifted if shifted as key pressed
 *
 * @retval 0 a string matched with (code1, code2)
 * @retval 1 no string matched with (code1, code2), but one matched with (code1,
 * CODE_NA)
 * @retval 2 no string matched with both of above are not found, ERROR (not
 * should be occured)
 */
uint8_t shingeta_send_string(uint8_t code1, uint8_t code2, bool is_shifted) {
  char string_to_send[SHINGETA_STRING_MAX_LENGTH *
                      2];  // length is doubled because "xtu" can be inserted to
                           // front
  int search_retv;

  search_retv = search_string_by_code(code1, code2, string_to_send);
  // break function if no string found
  if (search_retv == 2) {
    return 2;
  }

  // check if string is command
  // then do command and return
  for (size_t i = 0; i < COMMAND_NUM; i++) {
    if (strcmp(string_to_send, commands[i]) == 0) {
      functions_for_command[i](string_to_send, is_shifted);
      return search_retv;
    }
  }

  // if given string is not command
  // then just send string as normal
  send_normal_string(string_to_send, is_shifted);
  return search_retv;
}

/**
 * @brief function called when new shingeta key is pushed down
 * deal keys' push down with assigning or deleting shingeta_record or sending
 * string
 *
 * at first check if last_sgta_record is not expired with flag and
 * timestamp
 *
 * if it is enabled, make pair of record with it and given record
 * then send string that match to pair
 * if both code of pair is used, left no code in last_sgta_record
 * (uncheck flag alive) otherwise, given record will be new
 * last_sgta_record
 *
 * if it is disabled, just set given record to be new last_sgta_record
 * send string that match to old last_sgta_record if elapsed time is
 * reason of expirement
 *
 * @sa shingeta_send_string
 *
 * @param shingeta_record pushed down shingeta_record_t
 */
void on_push_shingeta_key(shingeta_record_t *shingeta_record) {
  if (last_sgta_record.is_alive) {
    // if unreleased key is exist previously
    bool is_last_key_expired =
        timer_elapsed(last_sgta_record.record.event.time) > SHINGETA_TAP_TERM;
    if (!is_last_key_expired) {
      // if unreleased key is not expired
      // in other word, if given record maybe second record of pair

      uint8_t is_two_codes_used;
      uint8_t code1 = get_code_from_shingeta_record(&last_sgta_record);
      uint8_t code2 = get_code_from_shingeta_record(shingeta_record);
      bool is_shifted =
          last_sgta_record.is_shifted && shingeta_record->is_shifted;

      is_two_codes_used = shingeta_send_string(code1, code2, is_shifted);
      if (is_two_codes_used == 0) {
        // if string matched (code1, code2) is found
        // remain no record alive
        last_sgta_record.is_alive = false;
      } else {
        // if string matched (code1, code2) is not found,
        // register given record for next input
        last_sgta_record = *shingeta_record;
      }
      return;
    } else {
      // if unreleased key is expired because of time
      // send string with unreleased key then register given record for next
      // input
      uint8_t code = get_code_from_shingeta_record(&last_sgta_record);
      shingeta_send_string(code, -1, last_sgta_record.is_shifted);

      last_sgta_record = *shingeta_record;
      return;
    }
  } else {
    // if unreleased key is not exist
    // just register given record for next input
    last_sgta_record = *shingeta_record;
    return;
  }
}

/**
 * @brief send string with last_key_record if alive, then disable it
 */
void flush_alive_last_shingeta_record(void) {
  if (last_sgta_record.is_alive) {
    uint8_t code = get_code_from_shingeta_record(&last_sgta_record);
    shingeta_send_string(code, -1, last_sgta_record.is_shifted);

    last_sgta_record.is_alive = false;
  }
}

/**
 * @brief function called when shingeta key is released
 * if last_key_record same as released key is still be remained,
 * send its string and disable it
 *
 * @param shingeta_record released key record
 */
void on_release_shingeta_key(shingeta_record_t *shingeta_record) {
  if (KEYEQ(last_sgta_record.record.event.key,
            shingeta_record->record.event.key)) {
    flush_alive_last_shingeta_record();
  }
}

/**
 * @brief initialize variables show shingeta record
 */
void init_shingeta_records(void) {
  reset_shingeta_record(&last_sgta_record);

  return;
}

// define functions to be called from other files
// ==============================================================
// from here

void init_shingeta_mode(void) {
  init_shingeta_code();
  init_shingeta_records();
  return;
}

void on_enter_shingeta_mode(void) {
  init_shingeta_records();
  return;
}

void on_exit_shingeta_mode(void) {
  flush_alive_last_shingeta_record();

  if (xtu_mode == SINGLE_XTU) {
    SEND_STRING("xtu");
  }
  xtu_mode = NO_XTU;
  return;
}

#ifdef SHINGETA_DEBUG_ENABLE
void debug_shingeta() {
  shingeta_record_t srec_a;
  shingeta_record_t srec_b;
  srec_a.record.event.key.row = 0x03;
  srec_a.record.event.key.col = 0x02;
  srec_b.record.event.key.row = 0x0b;
  srec_b.record.event.key.col = 0x01;

  // 0x32/0xb1 should be "mi"
  // (note) 0x32:"ka", 0xb1:"ga"
  srec_a.record.event.pressed = true;
  srec_a.record.event.time = timer_read();
  on_push_shingeta_key(&srec_a);
  srec_b.record.event.pressed = true;
  srec_b.record.event.time = timer_read();
  on_push_shingeta_key(&srec_b);
  srec_a.record.event.pressed = false;
  on_release_shingeta_key(&srec_a);
  srec_b.record.event.pressed = false;
  on_release_shingeta_key(&srec_b);

  SEND_STRING("\n");

  // two input separeted
  srec_a.record.event.pressed = true;
  srec_a.record.event.time = timer_read() - 100;
  on_push_shingeta_key(&srec_a);
  srec_b.record.event.pressed = true;
  srec_b.record.event.time = timer_read();
  on_push_shingeta_key(&srec_b);
  srec_a.record.event.pressed = false;
  on_release_shingeta_key(&srec_a);
  srec_b.record.event.pressed = false;
  on_release_shingeta_key(&srec_b);
}
#endif

/* define functions called in custom keycode
 * ==============================================================
 */

void shingeta_key(keyrecord_t *record, bool is_shifted) {
  // compose given keyrecord_t and other flags
  // into shingeta_record_t for give to other function
  shingeta_record_t new_shingeta_record;
  new_shingeta_record.record = *record;
  new_shingeta_record.is_shifted = is_shifted;
  new_shingeta_record.is_alive = true;

  if (record->event.pressed) {
#ifdef SHINGETA_DEBUG_ENABLE
    xprintf("pushed:\n");
    print_shingeta_record(&new_shingeta_record);
#endif
    on_push_shingeta_key(&new_shingeta_record);
  } else {
#ifdef SHINGETA_DEBUG_ENABLE
    xprintf("released:\n");
    print_shingeta_record(&new_shingeta_record);
#endif
    on_release_shingeta_key(&new_shingeta_record);
  }
  return;
}
