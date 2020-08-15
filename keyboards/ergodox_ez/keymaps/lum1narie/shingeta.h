/*
 * shingeta.h
 *
 *  Created on: 2018/06/02
 *      Author: lum1narie
 */

#ifndef SHIN_GETA_H_
#define SHIN_GETA_H_

#include <stdint.h>
#include <stdbool.h>
#include <quantum.h>

// interval(ms) to make judge two input as individual
#define SHINGETA_TAP_TERM 80

// max length of string send in single Shingeta input
#define SHINGETA_STRING_MAX_LENGTH 4

/* define when you want to show error code when shingeta key doesn't match
 * embed shingeta code
 */
#define SHINGETA_CODE_STRICT

#define ERROR_MESSAGE_MISMATCH_SHINGETA_CODE "!!! no string match to shingeta code !!!"

/**
 * @brief record if shifted or not and if be alive with keyrecord_t
 *
 * @param is_shifted record if shift(as layer modify) is pushed with record
 * @param is_alive when false, key record is ignored
 */
typedef struct {
    keyrecord_t record;
    bool is_shifted;
    bool is_alive;
} shingeta_record_t;

/**
 * @brief initialize this shingeta module
 *
 * supposed to be called at first
 */
extern void init_shingeta(void);
extern void on_enter_shingeta_mode(void);
extern void on_exit_shingeta_mode(void);

/**
 * @brief keycode procedure for shingeta normal key
 *
 * send string matched by tapped key
 * @param record tapped key record
 */
extern void shingeta_key(keyrecord_t*, bool);

#endif /* SHIN_GETA_H_ */
