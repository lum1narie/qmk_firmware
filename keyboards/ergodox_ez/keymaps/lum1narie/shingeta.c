/**
 * @file shingeta.c
 * @brief 新下駄入力の実装
 * @author lum1narie
 * @date 2018/06/02
 */
// call init_shingeta() at first of all
// call on_enter_shingeta_mode() when enter the shingeta mode and
// call on_exit_shingeta_mode() when exit the shingeta mode
#include QMK_KEYBOARD_H
#include "shingeta.h"
#include "ergodox_ez.h"
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdbool.h>
#include <quantum.h>
#include "shingeta_code.h"
#include "lum_util_qmk.h"

#ifdef SHINGETA_DEBUG_ENABLE
#include "print.h"
#endif

// keep keys being pressed while mod is active
shingeta_record_t last_pressed_sgta_record;

// marked as true when last input was "っ"(xtu)
volatile bool is_xtu_waiting = false;

// marked as true when all of last 2 inputs was "っ"(xtu)
volatile bool is_continuous_xtu_mode = false;

#ifdef SHINGETA_DEBUG_ENABLE
void print_shingeta_record(shingeta_record_t *shingeta_record)
{
    xprintf("record\n");
    xprintf("adress: %08lX\n", shingeta_record->record);
    xprintf("record.event.key.row: %X\n",
            shingeta_record->record.event.key.row);
    xprintf("record.event.key.col: %X\n",
            shingeta_record->record.event.key.col);
    xprintf("is_shifted: %X\nis_alive: %X\n",
            shingeta_record->is_shifted,
            shingeta_record->is_alive);
    xprintf("------------------------------\n");
    return;
}
#endif

/* define functions only for this file
 * ==============================================================
 */

/**
 * @brief reset flags in shingeta_record_t
 *
 * make is_shifted and is_alive false in record
 * @param shingeta_record record to reset
 */
void reset_shingeta_record(shingeta_record_t *shingeta_record) {
    shingeta_record->is_shifted = false;
    shingeta_record->is_alive = false;
    return;
}

/**
 * @brief convert keyrecord in shingeta_record_t into one 8bit number
 *
 * compress into one 8bit number so as make it easy to be decoded in string
 *
 * @param shingeta_record record to convert
 * @return code that is 8bit number
 */
uint8_t get_code_from_shingeta_record(shingeta_record_t *shingeta_record) {
    return position_to_8bit(shingeta_record->record.event.key);
}

/**
 * @brief send string which codes refers
 *
 * @param code1 former code, used in second try when mismatch
 * @param code2 latter code, treated as -1 in second try when mismatch
 * @param is_shifted if shifted in key pressed
 *
 * @retval 0 string matched (code1, code2) is found
 * @retval 1 string matched (code1, code2) is not found, one matched (code1, -1) is found
 * @retval 2 both of above are not found, ERROR (not should be occured)
 */
uint8_t shingeta_send_string(uint8_t code1, uint8_t code2, bool is_shifted) {
    char string_to_send[SHINGETA_STRING_MAX_LENGTH * 2]; // length is doubled because xtu can be inserted front
    int retv = 0;

#ifdef SHINGETA_DEBUG_ENABLE
    char debug_message[30];
    sprintf(debug_message, "code1: 0x%X, code2: 0x%X", (unsigned int)code1, (unsigned int)code2);
    send_string(debug_message);
#endif

    get_str_from_code(code1, code2, string_to_send);
    if (strcmp(string_to_send, "") == 0) {
        // when string matched (code1, code2) is not found
        retv = 1;
        get_str_from_code(code1, -1, string_to_send);

        // when string matched (code1, 0xFF) is not found
        // ERROR -- this should not be happen
        if (strcmp(string_to_send, "") == 0) {
            retv = 2;
#ifdef SHINGETA_CODE_STRICT
            SEND_STRING(ERROR_MESSAGE_MISMATCH_SHINGETA_CODE);
#endif
            return retv;
        }
    }

    // procedure for continuous xtu mode
    if (is_continuous_xtu_mode) {
        if (strcmp(string_to_send, COMMAND_4) == 0) {
            // enter "xtu" if another xtu code is given
            strcpy(string_to_send, "xtu");
            send_string(string_to_send);
            return retv;
        } else {
            // end continuous xtu mode and give code to procedure below
            is_continuous_xtu_mode = false;
        }
    }

    // check if string is command
    if (strcmp(string_to_send, COMMAND_1) == 0) {
        SEND_STRING("]\\"SS_TAP(X_LEFT)); // "[]"<- in JP
        return retv;
    } else if (strcmp(string_to_send, COMMAND_2) == 0) {
        SEND_STRING("*("SS_TAP(X_LEFT)); // "()"<- in JP
        return retv;
    } else if (strcmp(string_to_send, COMMAND_3) == 0) {
        SEND_STRING(SS_TAP(X_INT1)); // "/" in JP
        return retv;
    } else if (strcmp(string_to_send, COMMAND_4) == 0) {
        // if xtu code is given
        if (!is_xtu_waiting) {
            is_xtu_waiting = true;
            return retv;
        }
    } else if (strcmp(string_to_send, COMMAND_5) == 0) {
        if (is_shifted){
            SEND_STRING("Q>");
        } else {
            SEND_STRING(">");
        }
        return retv;
    }

    // when last input is single xtu
    if (is_xtu_waiting) {
        if (strcmp(string_to_send, COMMAND_4) == 0) {
            // enable continuous xtu mode if another xtu is given
            strcpy(string_to_send, "xtu");
            is_continuous_xtu_mode = true;
        } else {
            // just give xtu in front of given code
            xtu_letter(string_to_send);
        }
        is_xtu_waiting = false;
    }

    // shift capital letter if with flag (for SKK)
    if (is_shifted) {
        shift_capital_letter(string_to_send);
    }
    send_string(string_to_send);
    return retv;
}

/**
 * @brief function called when new shingeta key is pushed down
 * treat keys' push down with recording or deleting shingeta_record or sending string
 *
 * at first check if last_pressed_sgta_record is not expired with flag and timestamp
 *
 * if it is enabled, make pair of record with it and given record
 * then send string that match to pair
 * if both code of pair is used, left no code in last_pressed_sgta_record (uncheck flag alive)
 * otherwise, given record will be new last_pressed_sgta_record
 *
 * if it is disabled, just set given record will be new last_pressed_sgta_record
 * send string that match to old last_pressed_sgta_record if elapsed time is reason of expirement
 *
 * @sa shingeta_send_string
 *
 * @param shingeta_record pushed down shingeta_record_t
 */
void register_shingeta_record(shingeta_record_t *shingeta_record) {
    if (last_pressed_sgta_record.is_alive) {
        // if unreleased key is exist previously
        if (timer_elapsed(
                last_pressed_sgta_record.record.event.time) <= SHINGETA_TAP_TERM) {
            // if unreleased key is not expired
            // in other word, if given record maybe second record of pair

            uint8_t is_two_codes_used;
            uint8_t code1 = get_code_from_shingeta_record(
                    &last_pressed_sgta_record);
            uint8_t code2 = get_code_from_shingeta_record(shingeta_record);
            bool is_shifted = last_pressed_sgta_record.is_shifted
                    && shingeta_record->is_shifted;

            is_two_codes_used = shingeta_send_string(code1, code2, is_shifted);
            if (is_two_codes_used == 0) {
                // if string matched (code1, code2) is found
                last_pressed_sgta_record.is_alive = false;
            } else {
                // if string matched (code1, code2) is not found, register given record for next imput
                last_pressed_sgta_record = *shingeta_record;
            }
            return;
        } else {
            // if unreleased key is expired because of time
            // send string with unreleased key then register given record for next imput
            uint8_t code = get_code_from_shingeta_record(
                    &last_pressed_sgta_record);
            shingeta_send_string(code, -1, last_pressed_sgta_record.is_shifted);

            last_pressed_sgta_record = *shingeta_record;
            return;
        }
    } else {
        // if unreleased key is not exist
        // just register given record for next imput
        last_pressed_sgta_record = *shingeta_record;
        return;
    }
}

/**
 * @brief send string with last_key_record if alive then disable it
 */
void flush_alive_last_key_record(void) {
    if (last_pressed_sgta_record.is_alive) {
        uint8_t code = get_code_from_shingeta_record(&last_pressed_sgta_record);
        shingeta_send_string(code, -1, last_pressed_sgta_record.is_shifted);

        last_pressed_sgta_record.is_alive = false;
    }
}

/**
 * @brief function called when shingeta key is released
 * if last_key_record same as released key is still be remained,
 * send its string and disable it
 *
 * @param shingeta_record released key record
 */
void release_shingeta_record(shingeta_record_t *shingeta_record) {
    if (KEYEQ(last_pressed_sgta_record.record.event.key,
            shingeta_record->record.event.key)) {
        flush_alive_last_key_record();
    }
}

/**
 * @brief initialize variables show shingeta record
 */
void init_shingeta_records(void) {
    reset_shingeta_record(&last_pressed_sgta_record);

    return;
}

/* define functions to be called from other files
 * ==============================================================
 */

void init_shingeta(void) {
    init_shingeta_code();
    init_shingeta_records();
    return;
}

void on_enter_shingeta_mode(void) {
    init_shingeta_records();
    return;
}

void on_exit_shingeta_mode(void) {
    flush_alive_last_key_record();

    if (is_xtu_waiting) {
        SEND_STRING("xtu");
        is_xtu_waiting = false;
    }
    is_continuous_xtu_mode = false;
    return;
}

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
        register_shingeta_record(&new_shingeta_record);
    } else {
        release_shingeta_record(&new_shingeta_record);
    }
    return;
}
