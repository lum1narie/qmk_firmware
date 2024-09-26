#ifndef CONFIG_USER_H
#define CONFIG_USER_H

#include "../../config.h"

#define MOUSEKEY_INTERVAL       20
#define MOUSEKEY_DELAY          0

#undef DEBOUNCE
#define DEBOUNCE                50

#undef MOUSEKEY_TIME_TO_MAX
#define MOUSEKEY_TIME_TO_MAX    7

#undef MOUSEKEY_MAX_SPEED
#define MOUSEKEY_MAX_SPEED      5

#undef MOUSEKEY_WHEEL_DELAY
#define MOUSEKEY_WHEEL_DELAY    0

#define TAPPING_TOGGLE          1
#undef TAPPING_TERM
#define TAPPING_TERM            200
#define PERMISSIVE_HOLD

#undef LED_BRIGHTNESS_LO
#define LED_BRIGHTNESS_LO       5
#undef LED_BRIGHTNESS_HI
#define LED_BRIGHTNESS_HI       30
#undef LED_BRIGHTNESS_DEFAULT
#define LED_BRIGHTNESS_DEFAULT  15

// activate when debug keymap.c
// #define LUM_DEBUG_ENABLE
// activate when debug around Shingeta input
// #define SHINGETA_DEBUG_ENABLE

#endif
