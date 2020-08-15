/*
 * string_util.h
 *
 *  Created on: 2019/12/26
 *      Author: lum1narie
 */

#ifndef LUM_UTIL_QMK_H_
#define LUM_UTIL_QMK_H_

#include <stdint.h>
#include <quantum.h>

inline uint8_t position_to_8bit(keypos_t pos) {
    return (pos.row & 0x0f) << 4 | (pos.col & 0x0f);
}

/**
 * @brief make string with っ(xtu) on head
 *
 * ex)
 * + a -> xtu_a (n?[aiueon])
 * + ka -> k_ka (others)
 *
 * @param given_str string to be converted
 * @return
 */
extern char* xtu_letter(char*);

/**
 * @brief make capital letter upper case
 *
 * convert the first letter of given string to upper case if it is alphabet
 * supposed to be used for SKK (in which shift is trigger for Kanji-translation)
 *
 * also return same entity as given one
 *
 * @param given_str string to be converted
 * @return converted string
 */
extern char* shift_capital_letter(char*);

#endif /* LUM_UTIL_QMK_H_ */
