#include QMK_KEYBOARD_H
#include "keymap.h"

#include <avr/pgmspace.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "action_layer.h"
#include "ergodox_ez.h"
#include "keymap_jp.h"  // at quantum\keymap_extras
#include "quantum.h"
#include "shingeta.h"
#include "wait.h"

// supposed to used as ja-jp input keyboaord

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* TODO: implement M layer
     * BEAKL 15
     * 40123 76598
     * QHOUX GCRFZ
     * YIEA. DSTNB
     * J;,K' WMLPV
     */

    // BEAKL 15 layout
    [BK15] = LAYOUT_ergodox(
            // left hand
            KC_ESC,   KC_4,    KC_0,    KC_1,    KC_2,  KC_3,    JP_DLR,
            KC_TAB,   KC_Q,    KC_H,    KC_O,    KC_U,  KC_X,    JP_MINS,
            MO(LYRS), KC_Y,    KC_I,    KC_E,    KC_A,  JP_DOT,
            KC_LSFT,  KC_J,    JP_SCLN, JP_COMM, KC_K,  JP_QUOT, KC_LGUI,
            KC_LCTL,  JP_SLSH, JP_BSLS, JP_HASH, LCA_T(XXXXXXX),
                                                        KC_HOME, KC_PGDN,
                                                                 KC_LALT,
                                       LT(B15S,KC_SPC), KC_ENT,  KC_LCTL,
            // right hand
                JP_AMPR, KC_7, KC_6,    KC_5,    KC_9,    KC_8, JP_ZHTG,
                JP_AT,   KC_G, KC_C,    KC_R,    KC_F,    KC_Z, XXXXXXX,
                         KC_D, KC_S,    KC_T,    KC_N,    KC_B, KC_BSPC,
                KC_RGUI, KC_W, KC_M,    KC_UP,   KC_L,    KC_P, KC_DEL,
                               KC_LEFT, KC_DOWN, KC_RGHT, KC_V, KC_RCTL,
                KC_PGUP, KC_END,
                KC_RALT,
                KC_RCTL, KC_ENT,  LT(B15S,KC_SPC)
        ),

    [B15S] = LAYOUT_ergodox(
            // left hand
            S(KC_ESC),  JP_LBRC, JP_LCBR, JP_RCBR, JP_LPRN, JP_EQL,  JP_TILD,
            S(KC_TAB),  S(KC_Q), S(KC_H), S(KC_O), S(KC_U), S(KC_X), JP_UNDS,
            MO(LYRS),   S(KC_Y), S(KC_I), S(KC_E), S(KC_A), JP_GT,
            S(KC_LSFT), S(KC_J), JP_COLN, JP_LT,   S(KC_K), JP_DQT,  S(KC_LGUI),
            S(KC_LCTL), JP_QUES, JP_PIPE, JP_GRV,  MEH_T(XXXXXXX),
                                                         S(KC_HOME), S(KC_PGDN),
                                                                     S(KC_LALT),
                                                _______, S(KC_ENT),  S(KC_LCTL),
            // right hand
                JP_PERC,    JP_ASTR,    JP_RPRN,    JP_PLUS,    JP_RBRC, JP_EXLM, _______,
                JP_CIRC,    S(KC_G),    S(KC_C),    S(KC_R),    S(KC_F), S(KC_Z), _______,
                            S(KC_D),    S(KC_S),    S(KC_T),    S(KC_N), S(KC_B), S(KC_BSPC),
                S(KC_RGUI), S(KC_W),    S(KC_M),    S(KC_UP),   S(KC_L), S(KC_P), S(KC_DEL),
                            S(KC_LEFT), S(KC_DOWN), S(KC_RGHT), S(KC_V), _______,
                S(KC_PGUP), S(KC_END),
                S(KC_RALT),
                S(KC_RCTL), S(KC_ENT), _______
        ),

    /* qgmlwy layer
    *
    * ,---------------------------------------------------.           ,--------------------------------------------------.
    * | Esc     |   [  |   {  |   }  |   (  |   =  |  $   |           |  &   |   *  |   )  |   +  |   }  |   !  | Zenkaku|
    * |---------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
    * | Tab     |   Q  |   G  |   M  |   L  |   W  |  -   |           |  @   |   Y  |   F  |   U  |   B  |   ;  | ***    |
    * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
    * | L4/L6   |   D  |   S  |   T  |   N  |   R  |------|           |------|   I  |   A  |   E  |   O  |   H  | BS     |
    * |---------+------+------+------+------+------| Super|           | Super|------+------+------+------+------+--------|
    * | LShift  |   Z  |   X  |   C  |   V  |   J  |      |           |      |   K  |   P  |  Up  |   ,  |   .  | Delete |
    * `---------+------+------+------+------+-------------'           `-------------+------+------+------+------+-------'
    *   | LCtrl |  /   |  \   |  #   | C-A  |                                       | Left | Down | Right|   '  | L2     |
    *   `-----------------------------------'                                       `----------------------------------'
    *                                        ,-------------.       ,-------------.
    *                                        | Home | PgDn |       | PgUp | End  |
    *                                 ,------|------|------|       |------+------+------.
    *                                 |      |      | Alt  |       | Alt  |      |      |
    *                                 | Space|Enter |------|       |------|Enter |Space |
    *                                 |  L1  |      | Ctrl |       | Ctrl |      | L1   |
    *                                 `--------------------'       `--------------------'
    */
    [QGML] = LAYOUT_ergodox(
            // left hand
            KC_ESC,   JP_LBRC, JP_LCBR, JP_RCBR, JP_LPRN, JP_EQL, JP_DLR,
            KC_TAB,   KC_Q,    KC_G,    KC_M,    KC_L,    KC_W,   JP_MINS,
            MO(LYRS), KC_D,    KC_S,    KC_T,    KC_N,    KC_R,
            KC_LSFT,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_J,   KC_LGUI,
            KC_LCTL,  JP_SLSH, JP_BSLS, JP_HASH, LCA_T(XXXXXXX),
                                                         KC_HOME, KC_PGDN,
                                                                  KC_LALT,
                                        LT(QGMS,KC_SPC), KC_ENT,  KC_LCTL,
            // right hand
                JP_AMPR, JP_ASTR, JP_RPRN, JP_PLUS, JP_RBRC, JP_EXLM, JP_ZHTG,
                JP_AT,   KC_Y,    KC_F,    KC_U,    KC_B,    JP_SCLN, XXXXXXX,
                         KC_I,    KC_A,    KC_E,    KC_O,    KC_H,    KC_BSPC,
                KC_RGUI, KC_K,    KC_P,    KC_UP,   JP_COMM, JP_DOT,  KC_DEL,
                                  KC_LEFT, KC_DOWN, KC_RGHT, JP_QUOT, KC_RCTL,
                KC_PGUP, KC_END,
                KC_RALT,
                KC_RCTL, KC_ENT,  LT(QGMS,KC_SPC)
        ),

    /* shifted qgmlwy layer
     *
     * ,---------------------------------------------------.           ,--------------------------------------------------.
     * |         |   1  |   2  |   3  |   4  |   5  |  ~   |           |  %   |   6  |   7  |   8  |   9  |   0  |        |
     * |---------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
     * |         |   Q  |   G  |   M  |   L  |   W  |  _   |           |  ^   |   Y  |   F  |   U  |   B  |   :  |        |
     * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
     * | ***     |   D  |   S  |   T  |   N  |   R  |------|           |------|   I  |   A  |   E  |   O  |   H  |        |
     * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
     * |         |   Z  |   X  |   C  |   V  |   J  |      |           |      |   K  |   P  |      |   <  |   >  |        |
     * `---------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
     *   |       |  ?   |  |   |  `   |      |                                       |      |      |      |   "  | ***    |
     *   `-----------------------------------'                                       `------------------------------------'
     *                                        ,-------------.       ,-------------.
     *                                        |      |      |       |      |      |
     *                                 ,------|------|------|       |------+------+------.
     *                                 |      |      |      |       |      |      |      |
     *                                 |      |      |------|       |------|      |      |
     *                                 |      |      |      |       |      |      |      |
     *                                 `--------------------'       `--------------------'
     */
    [QGMS] = LAYOUT_ergodox(
            // left hand
            S(KC_ESC),  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    JP_TILD,
            _______,    S(KC_Q), S(KC_G), S(KC_M), S(KC_L), S(KC_W), JP_UNDS,
            XXXXXXX,    S(KC_D), S(KC_S), S(KC_T), S(KC_N), S(KC_R),
            _______,    S(KC_Z), S(KC_X), S(KC_C), S(KC_V), S(KC_J), S(KC_LGUI),
            S(KC_LCTL), JP_QUES, JP_PIPE, JP_GRV,  MEH_T(XXXXXXX),
                                                         S(KC_HOME), S(KC_PGDN),
                                                                     S(KC_LALT),
                                                _______, S(KC_ENT),  S(KC_LCTL),
            // right hand
                JP_PERC,    KC_6,    KC_7,       KC_8,       KC_9,       KC_0,    _______,
                JP_CIRC,    S(KC_Y), S(KC_F),    S(KC_U),    S(KC_B),    JP_COLN, _______,
                            S(KC_I), S(KC_A),    S(KC_E),    S(KC_O),    S(KC_H), S(KC_BSPC),
                S(KC_RGUI), S(KC_K), S(KC_P),    S(KC_UP),   JP_LT,      JP_GT,   S(KC_DEL),
                                     S(KC_LEFT), S(KC_DOWN), S(KC_RGHT), JP_DQT,  XXXXXXX,
                S(KC_PGUP), S(KC_END),
                S(KC_RALT),
                S(KC_RCTL), S(KC_ENT), _______
        ),

    /* qwerty layer
    *
    * ,---------------------------------------------------.           ,--------------------------------------------------.
    * | Esc     |   1  |   2  |   3  |   4  |   5  |  [   |           |  ]   |   6  |   7  |   8  |   9  |   0  | Zenkaku|
    * |---------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
    * | Tab     |   Q  |   W  |   E  |   R  |   T  |  -   |           |  '   |   Y  |   U  |   I  |   O  |   P  | Del    |
    * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
    * | L4/L6   |   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |   ;  | BS     |
    * |---------+------+------+------+------+------| Super|           | Super|------+------+------+------+------+--------|
    * | LShift  |   Z  |   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |  Up  |   ,  |   .  | RShift |
    * `---------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
    *   | LCtrl |  /   |  \   |  `   | C-A  |                                       | Left | Down | Right|   +  | L2     |
    *   `-----------------------------------'                                       `------------------------------------'
    *                                        ,-------------.       ,-------------.
    *                                        | Home | PgDn |       | PgUp | End  |
    *                                 ,------|------|------|       |------+------+------.
    *                                 |      |      | Alt  |       | Alt  |      |      |
    *                                 | Space|Enter |------|       |------|Enter |Space |
    *                                 |      | L3   | Ctrl |       | Ctrl |      | L3   |
    *                                 `--------------------'       `--------------------'
    */
    [QWER] = LAYOUT_ergodox(
            // left hand
            KC_ESC,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    JP_LBRC,
            KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    JP_MINS,
            MO(LYRS), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,
            KC_LSFT,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_LGUI,
            KC_LCTRL, JP_SLSH, JP_BSLS, JP_GRV,  LCA_T(XXXXXXX),
                                                          KC_HOME, KC_PGDN,
                                                                   KC_LALT,
                                                  KC_SPC, KC_ENT,  KC_LCTL,
            // right hand
                JP_RBRC, KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    JP_ZHTG,
                JP_QUOT, KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_DEL,
                         KC_H,    KC_J,    KC_K,    KC_L,    JP_SCLN, KC_BSPC,
                KC_RGUI, KC_N,    KC_M,    KC_UP,   JP_COMM, JP_DOT,  KC_RSFT,
                                  KC_LEFT, KC_DOWN, KC_RGHT, JP_PLUS, KC_RCTL,
                KC_PGUP, KC_END,
                KC_RALT,
                KC_RCTL, KC_ENT,  LT(QWES,KC_SPC)
       ),
    /* shifted qwerty layer
     *
     * ,---------------------------------------------------.           ,--------------------------------------------------.
     * |         |   !  |   @  |   #  |   $  |   %  |  {   |           |  }   |   ^  |   &  |   *  |   (  |   )  |        |
     * |---------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
     * |         |   Q  |   W  |   E  |   R  |   T  |  _   |           |  "   |   Y  |   U  |   I  |   O  |   P  |        |
     * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
     * | ***     |   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |   :  |        |
     * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
     * |         |   Z  |   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |      |   <  |   >  |        |
     * `---------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
     *   |       |  ?   |  |   |  ~   |      |                                       |      |      |      |   =  | ***    |
     *   `-----------------------------------'                                       `------------------------------------'
     *                                        ,-------------.       ,-------------.
     *                                        |      |      |       |      |      |
     *                                 ,------|------|------|       |------+------+------.
     *                                 |      |      |      |       |      |      |      |
     *                                 |      |      |------|       |------|      |      |
     *                                 |      |      |      |       |      |      |      |
     *                                 `--------------------'       `--------------------'
     */
    [QWES] = LAYOUT_ergodox(
            // left hand
            S(KC_ESC),  JP_EXLM, JP_AT,   JP_HASH, JP_DLR,  JP_PERC, JP_LCBR,
            _______,    S(KC_Q), S(KC_W), S(KC_E), S(KC_R), S(KC_T), JP_UNDS,
            XXXXXXX,    S(KC_A), S(KC_S), S(KC_D), S(KC_F), S(KC_G),
            _______,    S(KC_Z), S(KC_X), S(KC_C), S(KC_V), S(KC_B), S(KC_LGUI),
            S(KC_LCTL), JP_QUES, JP_PIPE, JP_TILD, MEH_T(XXXXXXX),
                                                         S(KC_HOME), S(KC_PGDN),
                                                                     S(KC_LALT),
                                              S(KC_SPC), S(KC_ENT),  S(KC_LCTL),
            // right hand
                JP_RCBR,    JP_CIRC, JP_AMPR,    JP_ASTR,    JP_LPRN,    JP_RPRN, _______,
                JP_QUOT,    S(KC_Y), S(KC_U),    S(KC_I),    S(KC_O),    S(KC_P), S(KC_DEL),
                            S(KC_H), S(KC_J),    S(KC_K),    S(KC_L),    JP_COLN, S(KC_BSPC),
                S(KC_RGUI), S(KC_N), S(KC_M),    S(KC_UP),   JP_LT,      JP_GT,   _______,
                                     S(KC_LEFT), S(KC_DOWN), S(KC_RGHT), JP_EQL,  XXXXXXX,
                S(KC_PGUP), S(KC_END),
                S(KC_RALT),
                S(KC_RCTL), S(KC_ENT), _______
    ),

    [SGTA] = LAYOUT_ergodox(  // shin geta
            // left hand
            _______, SGTAKEY, SGTAKEY, SGTAKEY, SGTAKEY, SGTAKEY, XXXXXXX,
            _______, SGTAKEY, SGTAKEY, SGTAKEY, SGTAKEY, SGTAKEY, LCTL(KC_J),
            OFFSGTA, SGTAKEY, SGTAKEY, SGTAKEY, SGTAKEY, SGTAKEY,
            _______, SGTAKEY, SGTAKEY, SGTAKEY, SGTAKEY, SGTAKEY, LCTL(KC_G),
            _______, JP_GT,   XXXXXXX, XXXXXXX, _______,
                                                         _______, _______,
                                                                  _______,
                                        LT(SGTS,KC_SPC), KC_ENT,  _______,
            // right hand
                XXXXXXX, SGTAKEY, SGTAKEY, SGTAKEY, SGTAKEY, SGTAKEY, _______,
                KC_L,    SGTAKEY, SGTAKEY, SGTAKEY, SGTAKEY, SGTAKEY, SGTAKEY,
                         SGTAKEY, SGTAKEY, SGTAKEY, SGTAKEY, SGTAKEY, KC_BSPC,
                KC_Q,    SGTAKEY, SGTAKEY, SGTAKEY, SGTAKEY, SGTAKEY, KC_DEL,
                                  KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______,
                _______, _______,
                _______,
                _______, KC_ENT,  LT(SGTS,KC_SPC)
        ),
    [SGTS] = LAYOUT_ergodox(  // shift on shin geta
            // left hand
            _______, SGKEYSF, SGKEYSF, SGKEYSF, SGKEYSF, SGKEYSF, XXXXXXX,
            _______, SGKEYSF, SGKEYSF, SGKEYSF, SGKEYSF, SGKEYSF, LCTL(KC_J),
            OFFSGTA, SGKEYSF, SGKEYSF, SGKEYSF, SGKEYSF, SGKEYSF,
            _______, SGKEYSF, SGKEYSF, SGKEYSF, SGKEYSF, SGKEYSF, LCTL(KC_G),
            _______, JP_GT,   XXXXXXX, XXXXXXX, _______,
                                                         _______, _______,
                                                                  _______,
                                                _______, KC_ENT,  _______,
            // right hand
                XXXXXXX, SGKEYSF, SGKEYSF, SGKEYSF, SGKEYSF, SGKEYSF, _______,
                S(KC_L), SGKEYSF, SGKEYSF, SGKEYSF, SGKEYSF, SGKEYSF, SGKEYSF,
                         SGKEYSF, SGKEYSF, SGKEYSF, SGKEYSF, SGKEYSF, KC_BSPC,
                S(KC_Q), SGKEYSF, SGKEYSF, SGKEYSF, SGKEYSF, SGKEYSF, KC_DEL,
                                  KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______,
                _______, _______,
                _______,
                _______, KC_ENT,  _______
        ),


    /* misc layer
     *
     * ,---------------------------------------------------.           ,-----------------------------------------------------.
     * | Esc     | F1   | F2   | F3   | F4   | F5   | F6   |           | F7   | F8   | F9   | F10  | F11  | F12  | Wake      |
     * |---------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
     * |         | ***  | Up   | ***  | P/B  | VUp  | PrSc |           | Insr | Tab  | 7    | 8    | 9    | *    |           |
     * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
     * |         | Left | Down | Right| Play | VDown|------|           |------| /    | 4    | 5    | 6    | -    |           |
     * |---------+------+------+------+------+------| SyRq |           | Caps |------+------+------+------+------+-----------|
     * | Shift   |LClick| MUp  |RClick| FF   | Mute |      |           |      |Nmlock| 1    | 2    | 3    | +    | Shift     |
     * `---------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
     *   |       | MLeft| MDown|MRight| Rew  |                                       | 0    | .    | ***  | Enter|         |
     *   `-----------------------------------'                                       `-------------------------------------'
     *                                        ,-------------.       ,-------------.
     *                                        |      |      |       |      |      |
     *                                 ,------|------|------|       |------+------+------.
     *                                 |      |      |      |       |      |      |      |
     *                                 | Space| Enter|------|       |------|Enter |Space |
     *                                 | Shift|      |      |       |      |      |Shift |
     *                                 `--------------------'       `--------------------'
     */
    [MISC] = LAYOUT_ergodox(
            // left hand
            KC_ESC,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,
            _______,  KC_BRID, KC_UP,   KC_BRIU, KC_PAUS, KC_VOLU, KC_PSCR,
            TG(MISC), KC_LEFT, KC_DOWN, KC_RGHT, KC_MPLY, KC_VOLD,
            _______,  KC_BTN1, KC_MS_U, KC_BTN2, KC_MNXT, KC_MUTE, KC_SYSREQ,
            _______,  KC_MS_L, KC_MS_D, KC_MS_R, KC_MPRV,
                                                          _______, _______,
                                                                   _______,
                                          LSFT_T(KC_SPC), KC_ENT,  _______,
            // right hand
                KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_WAKE,
                KC_INS,  KC_TAB,  KC_P7,   KC_P8,   KC_P9,   KC_PAST, _______,
                         KC_PSLS, KC_P4,   KC_P5,   KC_P6,   KC_PMNS, _______,
                KC_CAPS, KC_NLCK, KC_P1,   KC_P2,   KC_P3,   KC_PPLS, KC_RSFT,
                                  KC_P0,   KC_PDOT, XXXXXXX, KC_PENT, _______,
                _______, _______,
                _______,
                _______, KC_ENT,  RSFT_T(KC_SPC)
        ),

    [LYRS] = LAYOUT_ergodox( // layer switch
        // left hand
        RESET,   XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, DF(BK15), DF(QWER), DF(QGML), XXXXXXX, XXXXXXX, XXXXXXX,
        _______, ONSGTA,   TG(MISC), XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX,
                                                       XXXXXXX, XXXXXXX,
                                                                XXXXXXX,
                                              XXXXXXX, XXXXXXX, XXXXXXX,
        // right hand
            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                              XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
            XXXXXXX, XXXXXXX,
            XXXXXXX,
            XXXXXXX, XXXXXXX, XXXXXXX
        )
};
// clang-format on

#ifdef LUM_DEBUG_ENABLE
// ### DEBUG ###
// ========================================
/**
 * @brief send string shows active layers
 *
 * ex. when layer 0 3 12 is on, send "active layer: 03C"
 */
void send_layer(void) {
#define PREAMBLE_LAYER "active layer' "
  SEND_STRING(PREAMBLE_LAYER);

  for (uint8_t layer_no = 0; layer_no <= 31; layer_no++) {
    if (IS_LAYER_ON(layer_no)) {
      // send keycode if some layer is on
      uint8_t keycode_layer_no;
      if (layer_no == 0) {
        keycode_layer_no = (KC_0);
      } else if (layer_no < 10) {
        keycode_layer_no = (KC_Z + layer_no);
      } else {
        // {10 ~ 31} -> {'A' ~ 'V'}
        keycode_layer_no = (KC_A + layer_no - 10);
      }
      register_code(keycode_layer_no);
      unregister_code(keycode_layer_no);
    }
  }
  return;
}
// ========================================
#endif  // LUM_DEBUG_ENABLE

/**
 * @brief 5 times ESC to reset keyboard or just send ESC
 */
void esc_reset(qk_tap_dance_state_t *state, void *user_data) {
  if (state->count >= 5) {
    reset_keyboard();
  } else {
    for (int i = 0; i < state->count; ++i) {
      register_code(KC_ESC);
      wait_ms(10);
      unregister_code(KC_ESC);
      wait_ms(100);
    }
  }
}

/**
 * @brief single tap will enable layer SGTA and more taps will enable layer MISC
 */
void tgl_sgta_misc(qk_tap_dance_state_t *state, void *user_data) {
  if (state->count >= 2) {
    layer_on(MISC);
  } else {
    layer_on(SGTA);
    on_enter_shingeta_mode();
  }
}

// Tap Dance Definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_ESCRST] = ACTION_TAP_DANCE_FN(esc_reset),
    [TD_MODES] = ACTION_TAP_DANCE_FN(tgl_sgta_misc),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    // for Shingeta input
    // ========================================
    case CS_SGKEY:
      shingeta_key(record, false);
      return false;
    case CS_SGKEY_S:
      shingeta_key(record, true);
      return false;
      // ========================================

      // to enable/disable layers
      // ========================================
    case CS_ON_SGTA:
      if (record->event.pressed) {
        layer_on(SGTA);
        on_enter_shingeta_mode();
        return false;
      }
      break;
    case CS_OFF_SGTA:
      if (record->event.pressed) {
        layer_off(SGTA);
        on_exit_shingeta_mode();
        return false;
      }
      break;
      // ========================================

#ifdef LUM_DEBUG_ENABLE
    case CS_SENDLYR:
      if (record->event.pressed) {
        send_layer();
        return false;
      }
#endif  // LUM_DEBUG_ENABLE
  }
  return true;
}

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
  layer_clear();
  default_layer_set(INIT_LAYER_STATE);

  // init variables in shingeta
  init_shingeta_mode();
}

// refrence point of time when current brightness wave is start
uint16_t led_time_wave_start = 0;
// time elapsed from led_time_wave_start
uint16_t led_time_elapsed = 0;

// setting LED brightness
static inline void set_led_by_layer_state(void) {
  ergodox_right_led_1_off();
  ergodox_right_led_2_off();
  ergodox_right_led_3_off();
  ergodox_led_all_set(LED_SOLID_STRENGTH);

  // set led expressing each mode
  if (IS_LAYER_ON_STATE(default_layer_state, BK15)) {
    ergodox_right_led_1_on();
  }
  if (IS_LAYER_ON(SGTA)) {
    ergodox_right_led_2_on();
  }
  if (IS_LAYER_ON(MISC)) {
    ergodox_right_led_3_on();
  }

  led_time_elapsed = timer_elapsed(led_time_wave_start);
  // reset timer(led) for period
  if (led_time_elapsed > LED_BLINK_WAVE_DURATION_MS) {
    led_time_wave_start = timer_read();
    led_time_elapsed = 0;
  }

  // blink led if shifted
  if (IS_SHIFTED) {
    // position that means 0:bottom, LED_BLINK_WAVE_DURATION_MS:top
    uint16_t position_in_wave;

    if (IS_LAYER_OFF(B15S)) {
      // when shifted mode except BK15 mode
      position_in_wave =
          abs_diff(2 * led_time_elapsed, LED_BLINK_WAVE_DURATION_MS);
      ergodox_led_all_set(interior_division(
          (uint32_t)LED_BLINK_MIN_STRENGTH, (uint32_t)LED_BLINK_MAX_STRENGTH,
          position_in_wave, LED_BLINK_WAVE_DURATION_MS));
    } else {
      // when shifted in mode BK15
      ergodox_right_led_1_on();
      position_in_wave =
          abs_diff(2 * led_time_elapsed, LED_BLINK_WAVE_DURATION_MS);
      ergodox_led_all_set(
          interior_division((uint32_t)LED_WEAK_BLINK_MIN_STRENGTH,
                            (uint32_t)LED_WEAK_BLINK_MAX_STRENGTH,
                            position_in_wave, LED_BLINK_WAVE_DURATION_MS));
    }
  }

  if (IS_LAYER_ON(LYRS)) {
    // position that means 0:bottom, LED_BLINK_WAVE_DURATION_MS:top
    uint16_t position_in_wave =
        abs_diff(2 * led_time_elapsed, LED_BLINK_WAVE_DURATION_MS);
    ergodox_led_all_on();

    ergodox_led_all_set(interior_division(
        (uint32_t)LED_BLINK_MIN_STRENGTH, (uint32_t)LED_BLINK_MAX_STRENGTH,
        position_in_wave, LED_BLINK_WAVE_DURATION_MS));
  }
}

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) { set_led_by_layer_state(); }
