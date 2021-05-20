/**
 * @file str_utl.c
 * @brief qmkファームウェア用の汎用的な関数群
 * 文字列の処理とkeyrecordに関する関数群
 *
 * @author 2019/12/26
 * @date lum1narie
 */

#include QMK_KEYBOARD_H
#include "str_utl.h"

#include <avr/pgmspace.h>
#include <shingeta_code.h>
#include <stdio.h>

#include "ergodox_ez.h"

extern inline uint8_t position_to_8bit(keypos_t pos);

char* xtu_letter(char* given_str) {
  char capital_lower = given_str[0];
  if (capital_lower >= 'A' && capital_lower <= 'Z') {
    capital_lower -= ('A' - 'a');
  }
  if (capital_lower == 'b' || capital_lower == 'c' || capital_lower == 'd' ||
      capital_lower == 'f' || capital_lower == 'g' || capital_lower == 'h' ||
      capital_lower == 'j' || capital_lower == 'k' || capital_lower == 'm' ||
      capital_lower == 'p' || capital_lower == 'r' || capital_lower == 's' ||
      capital_lower == 't' || capital_lower == 'v' || capital_lower == 'w' ||
      capital_lower == 'y' || capital_lower == 'z') {
    // duplicate first letter
    // ex: "ka" -> "kka"
    char tmp_str[SHINGETA_STRING_MAX_LENGTH];
    strncpy(tmp_str, given_str, SHINGETA_STRING_MAX_LENGTH);
    tmp_str[SHINGETA_STRING_MAX_LENGTH - 1] = '\0';
    given_str[0] = capital_lower;
    given_str[1] = '\0';
    strcat(given_str, tmp_str);
  } else {
    // put "xtu" on head
    // ex: "a" -> "xtua"
    char tmp_str[SHINGETA_STRING_MAX_LENGTH];
    strncpy(tmp_str, given_str, SHINGETA_STRING_MAX_LENGTH);
    tmp_str[SHINGETA_STRING_MAX_LENGTH - 1] = '\0';
    strcpy(given_str, "xtu");
    strcat(given_str, tmp_str);
  }

  return given_str;
}

char* shift_capital_letter(char* given_str) {
  if (given_str[0] >= 'a' && given_str[0] <= 'z') {
    given_str[0] += 'A' - 'a';
  }
  return given_str;
}
