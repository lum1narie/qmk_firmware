/*
 * shingeta.h
 *
 *  Created on: 2018/06/02
 *      Author: lum1narie
 */

/* This code implement Shingeta Input that customized as compatible with SKK
 * shifted flag in some function will be used for Kanji conversion in SKK
 *
 * required_calling:
 * when you want to use shingeta mode, you must call some functions below
 * at the appropriate timing in keymap.c
 *
 * + init_shingeta(): when you boot keyboard
 * + on_enter_shingeta_mode(): when you entering in shingeta mode
 * + on_exit_shingeta_mode(): when you leaving from shingeta mode
 *
 *
 * usage:
 * register keycode which call shingeta_key(*record, is_shifted)
 * in keys you want use to imput shingeta
 * for exmaple, see how keymap.c in this directory is implemented
 *
 * when you desire to change which key to call shingeta_key(),
 * or to change what key sends what string, please see shingeta_code.h
 * then use script in ./util as written in it.
 */

#ifndef SHIN_GETA_H_
#define SHIN_GETA_H_

#include <quantum.h>
#include <stdbool.h>
#include <stdint.h>

#include "shingeta_code.h"
#include "str_utl.h"

// setting
// ============================================================
// from here

// interval(ms) to make judge two input as individual
#define SHINGETA_TAP_TERM 80

/* define when you want to show error code when no embed string match with
 * shingeta key this message will be shown as keyboard input, so please be
 * careful where you use this feature
 */
// #define SHINGETA_CODE_STRICT

// to here
// ============================================================

/*
 * error message shown when no matching string found when shingeta key is pushed
 * only shown when SHINGETA_CODE_STRICT is defined
 */
#define ERROR_MESSAGE_MISMATCH_SHINGETA_CODE \
  "!!! no string match to shingeta code !!!"

/*
 * state that show if xtu input is waiting
 * NO_XTU: normal mode
 * SINGLE_XTU: single xtu is input
 *     next input will be prefixed with xtu
 * CONTINUOUS_XTU: xtu is input two or more times
 */
typedef enum {
  NO_XTU,
  SINGLE_XTU,
  CONTINUOUS_XTU,
} xtu_mode_t;

/**
 * @brief in addition to keyrecord_t, record if it is shifted or not and if it
 * is alive
 *
 * @param is_shifted record if shift is pushed with record
 * @param is_alive when false, key record will be ignored
 */
typedef struct {
  keyrecord_t record;
  bool is_shifted;
  bool is_alive;
} shingeta_record_t;

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
static inline uint8_t get_code_from_shingeta_record(
    shingeta_record_t *shingeta_record) {
  return position_to_8bit(shingeta_record->record.event.key);
}

/**
 * @brief initialize this shingeta module
 *
 * supposed to be called at first
 */
extern void init_shingeta_mode(void);

/**
 * @brief ready up shingeta mode
 *
 * supposed to be called on entering in shingeta mode
 */
extern void on_enter_shingeta_mode(void);

/**
 * @brief clear temporary input when exiting from shingeta mode
 *
 * supposed to be called on exiting from shingeta mode
 */
extern void on_exit_shingeta_mode(void);

/**
 * @brief procedure on input of keycode of shingeta key
 *
 * this may send string matched by tapped key
 * @param record tapped key record
 * @param is_shifted
 *
 * @see shingeta_send_string()
 */
extern void shingeta_key(keyrecord_t* record, bool is_shifted);

#ifdef SHINGETA_DEBUG_ENABLE
extern void debug_shingeta(void);
#endif

#endif /* SHIN_GETA_H_ */
