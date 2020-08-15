/**
 * @file lum_util_qmk.c
 * @brief qmkファームウェア用の汎用的な関数群
 * 文字列の処理とkeyrecordに関する関数群
 *
 * @author 2019/12/26
 * @date lum1narie
 */

#include QMK_KEYBOARD_H
#include "lum_util_qmk.h"
#include "ergodox_ez.h"
#include "shingeta.h"
#include <avr/pgmspace.h>
#include <stdio.h>

extern inline uint8_t position_to_8bit(keypos_t pos);

char* xtu_letter(char *given_str) {
    if (given_str[0] == 'b' || given_str[0] == 'c' || given_str[0] == 'd'
            || given_str[0] == 'f' || given_str[0] == 'g' || given_str[0] == 'h'
            || given_str[0] == 'j' || given_str[0] == 'k' || given_str[0] == 'm'
            || given_str[0] == 'p' || given_str[0] == 'r' || given_str[0] == 's'
            || given_str[0] == 't' || given_str[0] == 'v' || given_str[0] == 'w'
            || given_str[0] == 'y' || given_str[0] == 'z') {
        // duplicate first letter
        // ex: "ka" -> "kka"
        char tmp_str[SHINGETA_STRING_MAX_LENGTH];
        strncpy(tmp_str, given_str, SHINGETA_STRING_MAX_LENGTH);
        tmp_str[SHINGETA_STRING_MAX_LENGTH - 1] = '\0';
        strncpy(given_str, tmp_str, 1);
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

char* shift_capital_letter(char *given_str) {
    if (given_str[0] >= 'a' && given_str[0] <= 'z') {
        given_str[0] += 'A' - 'a';
    }
    return given_str;
}

