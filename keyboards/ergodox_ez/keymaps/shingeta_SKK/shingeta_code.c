/*
 * shingeta_code.c
 *
 *  Created on: 2019/01/17
 *      Author: lum1narie
 */

/* when you need hash list with over 256*0.8 elements,
 * you need consider to change
 * + how to get_hash() (on shingeta_code.h)
 * + HASH_MAX
 *
 * when you need to make shingeta code from key patteren
 * modify util/shingeta_codelist_maker/code.py
 * then run util/shingeta_codelist_maker/shingeta_codelist_maker.py
 * apply codes in shingeta_code.txt into shingeta_codes in this file
 * and codes in shingeta_macro.txt into begining area of shingeta_code.h
 */

// call init_shingeta_code at first of all

#include QMK_KEYBOARD_H
#include "shingeta_code.h"

#include <avr/pgmspace.h>
#include <stdio.h>

#include "ergodox_ez.h"

volatile bool is_shingeta_code_ready = false;

// commands list
const char commands[COMMAND_NUM][SHINGETA_STRING_MAX_LENGTH] = {
    COMMAND_1, COMMAND_2, COMMAND_3, COMMAND_4, COMMAND_5};

// insert {-1, -1, ""} at last as watchdog
// clang-format off
const shingeta_assoc_t PROGMEM shingeta_codes[] = {
        {0x10, CODE_NA, "1"}, {0x11, CODE_NA, "-"}, {0x12, CODE_NA, "no"},
        {0x13, CODE_NA, "su"}, {0x20, CODE_NA, "2"}, {0x21, CODE_NA, "ni"},
        {0x22, CODE_NA, "to"}, {0x23, CODE_NA, "ma"}, {0x30, CODE_NA, "3"},
        {0x31, CODE_NA, "ha"}, {0x32, CODE_NA, "ka"}, {0x33, CODE_NA, "ki"},
        {0x40, CODE_NA, "4"}, {0x41, CODE_NA, ","}, {0x42, CODE_NA, "nn"},
        {0x43, CODE_NA, "ru"}, {0x50, CODE_NA, "5"}, {0x51, CODE_NA, "ti"},
        {0x52, CODE_NA, COMMAND_4}, {0x53, CODE_NA, "tu"},
        {0x80, CODE_NA, "6"}, {0x81, CODE_NA, "gu"}, {0x82, CODE_NA, "ku"},
        {0x83, CODE_NA, "te"}, {0x90, CODE_NA, "7"}, {0x91, CODE_NA, "ba"},
        {0x92, CODE_NA, "u"}, {0x93, CODE_NA, "ta"}, {0xa0, CODE_NA, "8"},
        {0xa1, CODE_NA, "ko"}, {0xa2, CODE_NA, "i"}, {0xa3, CODE_NA, "de"},
        {0xb0, CODE_NA, "9"}, {0xb1, CODE_NA, "ga"}, {0xb2, CODE_NA, "si"},
        {0xb3, CODE_NA, "."}, {0xc0, CODE_NA, "0"}, {0xc1, CODE_NA, "hi"},
        {0xc2, CODE_NA, "na"}, {0xc3, CODE_NA, "bu"}, {0xd1, CODE_NA, "ge"},
        {0x32, 0xd1, "uxo"}, {0x22, 0xc3, "che"}, {0x32, 0xc3, "vu"},
        {0x11, 0xc2, "a"}, {0x13, 0xc2, "h"}, {0x21, 0xc2, "s"},
        {0x22, 0xc2, "so"}, {0x23, 0xc2, "j"}, {0x31, 0xc2, "d"},
        {0x32, 0xc2, "e"}, {0x33, 0xc2, "k"}, {0x41, 0xc2, "f"},
        {0x43, 0xc2, "l"}, {0x51, 0xc2, "g"}, {0x53, 0xc2, COMMAND_5},
        {0x92, 0xc2, "x"}, {0x22, 0xc1, "je"}, {0x32, 0xc1, "we"},
        {0x22, 0xc0, "\""}, {0x32, 0xc0, "["}, {0x22, 0xb3, "po"},
        {0x32, 0xb3, "pu"}, {0x10, 0xb2, "xya"}, {0x11, 0xb2, "di"},
        {0x12, 0xb2, "wo"}, {0x13, 0xb2, "ze"}, {0x20, 0xb2, "mya"},
        {0x21, 0xb2, "me"}, {0x22, 0xb2, "sa"}, {0x23, 0xb2, "za"},
        {0x30, 0xb2, "myu"}, {0x31, 0xb2, "ke"}, {0x32, 0xb2, "o"},
        {0x33, 0xb2, "gi"}, {0x40, 0xb2, "myo"}, {0x41, 0xb2, "thi"},
        {0x42, 0xb2, "ri"}, {0x43, 0xb2, "ro"}, {0x50, 0xb2, "xwa"},
        {0x51, 0xb2, "dhi"}, {0x52, 0xb2, "zu"}, {0x53, 0xb2, "nu"},
        {0x10, 0xb1, "xyo"}, {0x11, 0xb1, "ryu"}, {0x12, 0xb1, "ryo"},
        {0x13, 0xb1, "rya"}, {0x20, 0xb1, "pya"}, {0x21, 0xb1, "ju"},
        {0x22, 0xb1, "ya"}, {0x30, 0xb1, "pyu"}, {0x31, 0xb1, "jo"},
        {0x32, 0xb1, "mi"}, {0x33, 0xb1, "ja"}, {0x40, 0xb1, "pyo"},
        {0x41, 0xb1, "gyu"}, {0x42, 0xb1, "gyo"}, {0x43, 0xb1, "gya"},
        {0x51, 0xb1, "nyu"}, {0x52, 0xb1, "nyo"}, {0x53, 0xb1, "nya"},
        {0x22, 0xb0, "'"}, {0x32, 0xb0, ";"}, {0x22, 0xa3, "pi"},
        {0x32, 0xa3, "be"}, {0x10, 0xa2, "xa"}, {0x11, 0xa2, "fa"},
        {0x12, 0xa2, "ho"}, {0x13, 0xa2, "du"}, {0x20, 0xa2, "xi"},
        {0x21, 0xa2, "go"}, {0x22, 0xa2, "zi"}, {0x23, 0xa2, "zo"},
        {0x30, 0xa2, "xu"}, {0x31, 0xa2, "hu"}, {0x32, 0xa2, "re"},
        {0x33, 0xa2, "bo"}, {0x40, 0xa2, "xe"}, {0x41, 0xa2, "fi"},
        {0x42, 0xa2, "mo"}, {0x43, 0xa2, "mu"}, {0x50, 0xa2, "xo"},
        {0x51, 0xa2, "fe"}, {0x52, 0xa2, "yu"}, {0x53, 0xa2, "fo"},
        {0x10, 0xa1, "xyu"}, {0x11, 0xa1, "hyu"}, {0x12, 0xa1, "hyo"},
        {0x13, 0xa1, "hya"}, {0x20, 0xa1, "bya"}, {0x21, 0xa1, "shu"},
        {0x22, 0xa1, "do"}, {0x30, 0xa1, "byu"}, {0x31, 0xa1, "sho"},
        {0x32, 0xa1, "yo"}, {0x33, 0xa1, "sha"}, {0x40, 0xa1, "byo"},
        {0x41, 0xa1, "kyu"}, {0x42, 0xa1, "kyo"}, {0x43, 0xa1, "kya"},
        {0x51, 0xa1, "chu"}, {0x52, 0xa1, "cho"}, {0x53, 0xa1, "cha"},
        {0x22, 0xa0, "("}, {0x32, 0xa0, "\\"}, {0x22, 0x93, "da"},
        {0x32, 0x93, "ne"}, {0x22, 0x92, "ra"}, {0x32, 0x92, "a"},
        {0x81, 0x92, "/"}, {0x82, 0x92, COMMAND_2}, {0x83, 0x92, "?"},
        {0x22, 0x91, "pe"}, {0x32, 0x91, "pa"}, {0x22, 0x90, "*"},
        {0x32, 0x90, "]"}, {0x22, 0x83, "wa"}, {0x32, 0x83, "se"},
        {0x22, 0x82, "bi"}, {0x32, 0x82, "he"}, {0x22, 0x81, "she"},
        {0x32, 0x81, "wi"}, {0x22, 0x80, "z/"}, {0x32, 0x80, "z."},
        {0x42, 0x53, "!"}, {0x42, 0x52, COMMAND_1}, {0x42, 0x51, COMMAND_3},
        {CODE_NA, CODE_NA, ""},
};
// clang-format on

int shingeta_codes_size;        // initialized in init_shingeta_code
int hash_index_list[HASH_MAX];  // initialized in init_shingeta_code

void init_shingeta_code(void) {
  for (int i = 0; i < HASH_MAX; ++i) {
    uint8_t lower_code = pgm_read_byte(&shingeta_codes[i].lower_code);
    uint8_t higher_code = pgm_read_byte(&shingeta_codes[i].higher_code);
    if (lower_code == CODE_NA && higher_code == CODE_NA) {
      break;
    }
    shingeta_codes_size++;
  }

  // when number of shingeta code is more than list length -- this should not be
  // happen
  if (shingeta_codes_size >= HASH_MAX - 1) {
    SEND_STRING(ERROR_MESSAGE_SHINGETA_CODE_SPILLED);
    return;
  }

  // initialize hash_index_list here
  for (int i = 0; i < HASH_MAX; ++i) {
    hash_index_list[i] = -1;
  }

  // register unhashed index number in hash_index_list
  for (int i = 0; i < shingeta_codes_size; ++i) {
    uint8_t lower_code = pgm_read_byte(&shingeta_codes[i].lower_code);
    uint8_t higher_code = pgm_read_byte(&shingeta_codes[i].higher_code);
    uint8_t initial_hash = get_hash(lower_code, higher_code);
    for (uint8_t j = initial_hash; j != ((initial_hash - 1) % HASH_MAX);
         j = ((j + 1) % HASH_MAX)) {
      if (hash_index_list[j] == -1) {
        hash_index_list[j] = i;
        break;
      }
    }
  }

  is_shingeta_code_ready = true;
  return;
}

char* get_str_from_code(uint8_t code1, uint8_t code2, char* ret_str) {
  if (!is_shingeta_code_ready) {
    strncpy(ret_str, "", 1);
    SEND_STRING(ERROR_MESSAGE_SHINGETA_CODE_NOT_READY);
    return ret_str;
  }

  uint8_t lower_code;
  uint8_t higher_code;
  uint8_t initial_hash;

  // register code as lower<-min, higher<-max
  if (code1 < code2) {
    lower_code = code1;
    higher_code = code2;
  } else {
    lower_code = code2;
    higher_code = code1;
  }

  initial_hash = get_hash(lower_code, higher_code);

  for (uint8_t i = initial_hash; i != ((initial_hash - 1) % HASH_MAX);
       i = ((i + 1) % HASH_MAX)) {
#define shingeta_code_registerd (shingeta_codes[hash_index_list[i]])
    uint8_t lower_code_registerd =
        pgm_read_byte(&shingeta_code_registerd.lower_code);
    uint8_t higher_code_registerd =
        pgm_read_byte(&shingeta_code_registerd.higher_code);

    if (lower_code == lower_code_registerd &&
        higher_code == higher_code_registerd) {
      strcpy_P(ret_str, shingeta_code_registerd.string);
      return ret_str;
    } else if (lower_code_registerd == CODE_NA) {
      // if both code are N/A
      strcpy(ret_str, "");
      return ret_str;
    }
  }
  strcpy(ret_str, "");
  return ret_str;
}
