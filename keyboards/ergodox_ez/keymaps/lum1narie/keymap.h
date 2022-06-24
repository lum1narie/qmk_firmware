/*
 * keymap.h
 *
 *  Created on: 2019/12/26
 *      Author: lum1narie
 */

#ifndef ERGODOX_EZ_LUM1NARIE_KEYMAP_H_
#define ERGODOX_EZ_LUM1NARIE_KEYMAP_H_

#include "quantum.h"


// keyboard setting for QMK implement
// ========================================
#define NO_ACTION_MACRO     // disable MACRO() & action_get_macro
#define NO_ACTION_FUNCTION  // disable action_function()
#define FORCE_NKRO
// ========================================

#define INIT_DEFAULT_LAYER  BK15
#define INIT_LAYER_STATE    (1<<INIT_DEFAULT_LAYER)

#define LED_SOLID_STRENGTH          15
#define LED_BLINK_MIN_STRENGTH      3
#define LED_BLINK_MAX_STRENGTH      40
#define LED_WEAK_BLINK_MIN_STRENGTH 3
#define LED_WEAK_BLINK_MAX_STRENGTH 10
#define LED_BLINK_WAVE_DURATION_MS  1200

// pick single bit in bits row
#define pick_bit_state(bits, n) ((bits) & (1UL << (n)))

/**
 * @brief calc |x - y| without using negative number
 * supposed to be used for unsigned value
 */
#define abs_diff(x, y) ((x) > (y) ? (x) - (y) : (y) - (x))

/**
 * @brief calc interior division point
 * distance (point - l) should be distance (r - l) * (pos / div)
 *
 * @arg l   left number
 * @arg r   right number
 * @arg pos position from left
 * @arg div position of right from left
 */
#define interior_division(l, r, pos, div) ((l * (div - pos) + r * pos) / div)

// if at least one of shifted layer is active
#define IS_SHIFTED (IS_LAYER_ON(QGMS) || IS_LAYER_ON(QWES))

enum layer_id {
    BEAKL15 = 0, // BEAKL15 layer (default)
    BEAKL15_S, // shift on BEAKL15
    QGMLWY,     // qgmlwy(carpalx) layer
    QGMLWY_S,   // shift on qgmlwy
    QWERTY,     // qwerty layer
    QWERTY_S,   // shift on qwerty
    SHIN_GETA,  // shin geta layer
    SHIN_GETA_S,    // shift on shin geta
    MISC,   // MISC
    LAYERS, // for switching layers
};

#define BK15 BEAKL15
#define B15S BEAKL15_S
#define QGML QGMLWY
#define QGMS QGMLWY_S
#define QWER QWERTY
#define QWES QWERTY_S
#define SGTA SHIN_GETA
#define SGTS SHIN_GETA_S
#define LYRS LAYERS

enum tap_dance_id {
    TD_ESCRST = 0, TD_MODES
};

enum custom_keycodes {
    CS_SGKEY = SAFE_RANGE,
    CS_SGKEY_S,
    CS_ON_SGTA,
    CS_OFF_SGTA,
    CS_SENDLYR, /*###DEBUG###*/
};

#define SGTAKEY CS_SGKEY
#define SGKEYSF CS_SGKEY_S
#define ONSGTA CS_ON_SGTA
#define OFFSGTA CS_OFF_SGTA
#define SENDLYR CS_SENDLYR


#endif /* ERGODOX_EZ_LUM1NARIE_KEYMAP_H_ */
