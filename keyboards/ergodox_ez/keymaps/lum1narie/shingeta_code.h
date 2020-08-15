/*
 * shingeta_code.h
 *
 *  Created on: 2019/01/17
 *      Author: lum1narie
 */

#ifndef SHINGETA_CODE_H_
#define SHINGETA_CODE_H_

#include "shingeta.h"

// TODO: leave documentation

// length of hash list
#define HASH_MAX 256

#define COMMAND_1 "c01" // hit [] then move cursor into bracket
#define COMMAND_2 "c02" // hit () then move cursor into bracket
#define COMMAND_3 "c03" // hit / for jp
#define COMMAND_4 "c04" // hit っ
#define COMMAND_5 "c05" // hit > if not shifted, hit Q> if shifted

// make hash that is 0x00 ~ 0xFF from two number(supposed to be uint8_t)
#define get_hash(c1, c2) (((uint16_t)(c1) * 40503 ^ (uint16_t)(c2) * 40503) >> 8)

#define ERROR_MESSAGE_SHINGETA_CODE_SPILLED "!!! embed shingeta code is too much than hash list !!!\n!!! shingeta mode disabled !!!"
#define ERROR_MESSAGE_SHINGETA_CODE_NOT_READY "!!! shingeta codes is not ready !!!"

// element of associantion map of shingeta codes to string
typedef struct {
    uint8_t lower_code;
    uint8_t higher_code;
    char string[SHINGETA_STRING_MAX_LENGTH];
} shingeta_assoc_t;

volatile bool is_shingeta_code_ready;

extern void init_shingeta_code(void);
extern char* get_str_from_code(uint8_t, uint8_t, char*);

#endif /* SHINGETA_CODE_H_ */
