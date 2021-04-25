/* Copyright 2019 Thomas Baart <thomas@splitkb.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

// PC comms script
#include "raw_hid.h"
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "keyboards/kyria/keymaps/manna-harbour_miryoku/config.h"
#include "keyboards/kyria/keymaps/manna-harbour_miryoku/keymap.c"

#include "print.h"

// Tap-hold mouse button
uint16_t mouse_button_timer = 0;

enum custom_keycodes {
  MOUSE_BTN = SAFE_RANGE
};

enum layers {
    _QWERTY = 0,
    _LOWER,
    _RAISE,
    _ADJUST
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: QWERTY
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |RAIS/ESC|   Q  |   W  |   E  |   R  |   T  |                              |   Y  |   U  |   I  |   O  |   P  |  | \   |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |Ctrl/BS |   A  |   S  |  D   |   F  |   G  |                              |   H  |   J  |   K  |   L  | ;  : |  ' "   |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  |LShift|LShift|  |LShift|LShift|   N  |   M  | ,  < | . >  | /  ? |  - _   |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        | GUI  | Del  | Enter| Space| Esc  |  | Enter| Space| Tab  | Bksp | AltGr|
 *                        |      |      | Alt  | Lower| Raise|  | Lower| Raise|      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_QWERTY] = LAYOUT(
      LT(_RAISE, KC_ESC),       KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,                                         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_PIPE,
      MT(MOD_LCTL, KC_BSPC),   KC_A,   KC_S,   KC_D,   KC_F,   KC_G,                                         KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
      KC_LSFT,                 KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,   KC_LSFT,   KC_LSFT, KC_LSFT, MOUSE_BTN, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_MINS,
              KC_LGUI, KC_DEL, MT(MOD_LALT, KC_ENT), LT(_LOWER, KC_SPC), LT(_RAISE, KC_ESC), LT(_LOWER, KC_ENT), LT(_RAISE, KC_SPC), KC_TAB,  KC_BSPC, KC_RALT
    ),
/*
 * Lower Layer: Symbols
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  !   |  @   |  {   |  }   |  |   |                              |      |      |      |      |      |  | \   |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  #   |  $   |  (   |  )   |  `   |                              |   +  |  -   |  /   |  *   |  %   |  ' "   |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  %   |  ^   |  [   |  ]   |  ~   |      |      |  |      |      |   &  |  =   |  ,   |  .   |  / ? | - _    |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |  ;   |  =   |  |  =   |  ;   |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_LOWER] = LAYOUT(
      _______, KC_EXLM, KC_AT,   KC_LCBR, KC_RCBR, KC_PIPE,                                     _______, _______, _______, _______, _______, KC_BSLS,
      _______, KC_HASH, KC_DLR,  KC_LPRN, KC_RPRN, KC_GRV,                                      KC_PLUS, KC_MINS, KC_SLSH, KC_ASTR, KC_PERC, KC_QUOT,
      _______, KC_PERC, KC_CIRC, KC_LBRC, KC_RBRC, KC_TILD, _______, _______, _______, _______, KC_AMPR, KC_EQL,  KC_COMM, KC_DOT,  KC_SLSH, KC_MINS,
                                 _______, _______, _______, KC_SCLN, KC_EQL,  KC_EQL,  KC_SCLN, _______, _______, _______
    ),
/*
 * Raise Layer: Number keys, media, navigation
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |   1  |  2   |  3   |  4   |  5   |                              |  6   |  7   |  8   |  9   |  0   |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      | Prev | Play | Next | VolUp|                              | Left | Down | Up   | Right|      |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      | Mute | VolDn|      |      |  |      |      | MLeft| Mdown| MUp  |MRight|      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_RAISE] = LAYOUT(
      _______, KC_1, 	  KC_2,    KC_3,    KC_4,    KC_5,                                        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______,
      _______, _______, KC_MPRV, KC_MPLY, KC_MNXT, KC_VOLU,                                     KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, _______,
      _______, _______, _______, _______, KC_MUTE, KC_VOLD, _______, _______, _______, _______, KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, _______, _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
/*
 * Adjust Layer: Function keys, RGB
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        | F1   |  F2  | F3   | F4   | F5   |                              | F6   | F7   |  F8  | F9   | F10  |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        | TOG  | SAI  | HUI  | VAI  | MOD  |                              |      |      |      | F11  | F12  |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      | SAD  | HUD  | VAD  | RMOD |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_ADJUST] = LAYOUT(
      _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                       KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  _______,
      _______, RGB_TOG, RGB_SAI, RGB_HUI, RGB_VAI, RGB_MOD,                                     _______, _______, _______, KC_F11,  KC_F12,  _______,
      _______, _______, RGB_SAD, RGB_HUD, RGB_VAD, RGB_RMOD,_______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
// /*
//  * Layer template
//  *
//  * ,-------------------------------------------.                              ,-------------------------------------------.
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  */
//     [_LAYERINDEX] = LAYOUT(
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
//                                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
//     ),
};

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

#ifdef INFO_OLED_ENABLE

// HID input, see https://github.com/BlankSourceCode/qmk_firmware/commit/3ae097783d65e71062606906f7b4be639d9d321d
bool is_hid_connected = false; // Flag indicating if we have a PC connection yet
bool is_screen_buffered = false; // Flag indicating if we have obtained a screen buffer
int screen_data_index = 0; // Current index into the screen_copy_buffer that we should write to
uint8_t screen_max_count = 0;  // Number of info screens we can scroll through (set by connecting node script)
uint8_t screen_show_index = 0; // Current index of the info screen we are displaying
char screen_copy_buffer[INFO_BUFFER_SIZE] = {0}; // Buffer used to store the screen data sent by connected node script
char screen_show_buffer[INFO_BUFFER_SIZE] = {0}; // Buffer used to store the final screen data

bool can_render_info_panel(void) {
  return is_screen_buffered && (screen_show_index != 0);
}

static void render_info_panel(void) {
  oled_set_cursor(0, 0);
  for (int i = 0; i < INFO_BUFFER_SIZE; ++i) {
    oled_write_char(screen_show_buffer[i], false);
  }
}

void raw_hid_send_screen_index(void) {
  // Send the current info screen index to the connected node script so that it can pass back the new data
  uint8_t send_data[32] = {0};
  send_data[0] = 1; // Packet type
  send_data[1] = screen_show_index; // First data byte is the current requested index;
  send_data[2] = oled_max_chars(); // Second byte is the screen width
  send_data[3] = oled_max_lines() - 2; // Third byte is the screen height

  raw_hid_send(send_data, sizeof(send_data));
}

void raw_hid_receive(uint8_t *data, uint8_t length) {
  // PC connected, so set the flag to show a message on the master display
  is_hid_connected = true;

  // Initial connections use '1' in the first byte to indicate this
  if (length > 0) {
    bool fill_buffer = false;

    if (data[0] == 1) {
      // New connection so restart screen_copy_buffer
      screen_data_index = 0;

      print("New connection");

      // The second byte is the number of info screens the connected node script allows us to scroll through
      screen_max_count = data[1];
      if (screen_show_index >= screen_max_count) {
        screen_show_index = 0;
      }

      // Tell the connection which info screen we want to look at initially
      raw_hid_send_screen_index();
      return;
    } else if (data[0] == 2) {
      // New screen buffer
      screen_data_index = 0;
      fill_buffer = true;

      print("New buffer");
    } else if (data[0] == 3) {
      // Continue filling the latest buffer
      fill_buffer = true;

      print("Filling buffer");
    }

    uprintf(" data_i=%d, fill=%d, packet=%d", screen_data_index, fill_buffer, data[0]);
  
    // Otherwise the data we receive is one line of the screen to show on the display
    if (fill_buffer) {
      // If copy index is at beginning, clear copy buffer
      if (screen_data_index == 0) {
        memset(&screen_copy_buffer[0], ' ', INFO_BUFFER_SIZE);
      }

      bool flush_buffer = false;

      // Deduct the first byte from length, it's the packet identifier. Also make it index-based
      length -= 2;

      if (screen_data_index + length >= INFO_BUFFER_SIZE) {
        flush_buffer = true;
        length = INFO_BUFFER_SIZE - screen_data_index; // Cap length
      }

      uprintf(", future_i=%d", screen_data_index + length);

      // Copy the data into our buffer and increment the number of lines we have got so far
      memcpy((char*)&screen_copy_buffer[screen_data_index], (char*)&data[1], length);
      screen_data_index += length;

      // Once we reach full buffer, we have a full screen
      if (flush_buffer) {
        // Reset the buffer back to receive the next full screen data
        screen_data_index = 0;

        // First clear the transfer buffer with spaces just in case.
        memset((char*)&screen_show_buffer, ' ', INFO_BUFFER_SIZE);

        // Copy in the screen data
        memcpy((char*)&screen_show_buffer, (char*)&screen_copy_buffer, INFO_BUFFER_SIZE);

        // Finally set the screen buffer flag so we can_render_info_panel()
        is_screen_buffered = true;

        print(", flushed!");

        uprintf("\nFlush data: \n%s\n%s\n", screen_copy_buffer, screen_show_buffer);
      }
    }
  }

  print("\n");
}

#endif // INFO_OLED_ENABLE

#ifdef BONGOCAT_ENABLE

uint32_t anim_timer = 0;
uint32_t anim_sleep = 0;
uint8_t current_idle_frame = 0;
// uint8_t current_prep_frame = 0; // uncomment if PREP_FRAMES >1
uint8_t current_tap_frame = 0;

char wpm_str[8];

// Images credit j-inc(/James Incandenza) and pixelbenny. Credit to obosob for initial animation approach.

static void render_anim(void)
{
  // saves the pixel index in bits 1-13 and pixel on, off in bit 16. 1: on 0: off (+2^15)
  // first element is the size of the array , without the size itself
  static const uint16_t base[] PROGMEM = {
    223, 32968, 32969, 32970, 33095, 33099, 33222, 33228, 33346, 33347, 33348, 33349, 33356, 33470, 33471, 33472, 
    33473, 33484, 33596, 33597, 33613, 33723, 33742, 33789, 33790, 33791, 33844, 33845, 33850, 33871, 33872, 33913, 
    33914, 33915, 33916, 33971, 33974, 33975, 33976, 33977, 34001, 34036, 34037, 34038, 34039, 34040, 34098, 34130, 
    34160, 34161, 34162, 34163, 34226, 34259, 34283, 34284, 34285, 34286, 34287, 34354, 34379, 34380, 34388, 34407, 
    34408, 34409, 34410, 34483, 34507, 34508, 34517, 34530, 34531, 34532, 34533, 34534, 34611, 34638, 34639, 34640, 
    34641, 34646, 34654, 34655, 34656, 34657, 34740, 34751, 34752, 34762, 34766, 34767, 34768, 34769, 34775, 34779, 
    34780, 34781, 34868, 34879, 34880, 34884, 34887, 34888, 34889, 34890, 34903, 34904, 34905, 34906, 34996, 35013, 
    35014, 35032, 35123, 35132, 35133, 35134, 35135, 35150, 35151, 35160, 35250, 35260, 35261, 35262, 35263, 35273, 
    35274, 35275, 35276, 35277, 35280, 35281, 35288, 35378, 35397, 35398, 35399, 35400, 35410, 35411, 35412, 35413, 
    35414, 35415, 35506, 35522, 35523, 35524, 35633, 35649, 35761, 35778, 35889, 35892, 35893, 35894, 35895, 35896, 
    35897, 35906, 36016, 36017, 36018, 36019, 36026, 36027, 36034, 36142, 36143, 36156, 36157, 36162, 36266, 36267, 
    36268, 36269, 36286, 36287, 36288, 36289, 36390, 36391, 36392, 36393, 36514, 36515, 36516, 36517, 36638, 36639, 
    36640, 36641, 36762, 36763, 36764, 36765, 36886, 36887, 36888, 36889, 37010, 37011, 37012, 37013, 37134, 37135, 
    37136, 37137, 37258, 37259, 37260, 37261, 37382, 37383, 37384, 37385, 37506, 37507, 37508, 37509, 37632, 37633
  };

  // diffs rendered after base frame, turns on/off certain pixels
  // unfortunately C Arrays can't have a variable second dimension
  static const uint16_t idle_diff_0[] PROGMEM = {0};
  static const uint16_t idle_diff_2[] PROGMEM = {22, 32841, 32842, 201, 202, 32971, 331, 33100, 588, 33357, 716, 33485, 33716, 33717, 33843, 1076, 1077, 33846, 33847, 33970, 1203, 1206, 1207};
  static const uint16_t idle_diff_3[] PROGMEM = {
    104, 200, 327, 33096, 454, 33223, 578, 579, 580, 581, 33350, 702, 703, 704, 705, 33474, 
    33475, 33476, 33477, 716, 33485, 828, 829, 33598, 33599, 33600, 33601, 955, 33724, 33725, 33741, 974, 
    1082, 33851, 33870, 1103, 1104, 1208, 1209, 33978, 33999, 34000, 1233, 34104, 34105, 34129, 1362, 34258, 
    1491, 1611, 1612, 34387, 1620, 34482, 1715, 34516, 1749, 34635, 34636, 1870, 1871, 1872, 1873, 34645, 
    1878, 34739, 1972, 1983, 1984, 1994, 34774, 2007, 2116, 2119, 2120, 2121, 34894, 34895, 34896, 34897, 
    35007, 35008, 35012, 2245, 2246, 35015, 35016, 35017, 35018, 2355, 35124, 2364, 2365, 2366, 2367, 35141, 
    35142, 2482, 35251, 35388, 35389, 35390, 35391, 2865, 35634
  };
  static const uint16_t idle_diff_4[] PROGMEM = {20, 200, 201, 202, 327, 33096, 33097, 33098, 331, 454, 33223, 33227, 460, 33350, 1076, 1077, 1203, 33972, 33973, 1330, 34099};
  static const uint16_t *idle_diff[IDLE_FRAMES] = {
      idle_diff_0,
      idle_diff_0, // first two frames are identical
      idle_diff_2,
      idle_diff_3,
      idle_diff_4
  };

  static const uint16_t prep_diff_0[] PROGMEM = {
    107, 32841, 32842, 201, 202, 32971, 331, 33100, 588, 33357, 716, 33485, 33716, 33717, 33843, 1076, 
    1077, 33846, 33847, 33875, 33876, 33877, 33878, 33970, 1203, 1206, 1207, 34002, 34007, 34133, 34136, 34257, 
    34263, 34264, 34385, 34389, 34392, 34512, 34516, 34520, 1870, 1871, 1873, 1878, 34648, 34746, 34747, 34748, 
    1998, 1999, 2001, 34873, 34875, 34877, 35000, 35005, 35027, 35028, 35029, 35030, 2264, 35128, 35129, 2365, 
    2367, 35152, 35153, 35154, 2392, 35255, 35258, 35259, 2492, 2493, 2495, 2512, 2513, 2520, 35383, 35386, 
    35387, 35390, 2642, 2643, 2644, 2645, 2646, 2647, 35518, 35646, 35647, 35648, 35770, 35771, 35772, 35773, 
    3010, 3138, 3258, 3259, 3266, 3388, 3389, 3394, 3518, 3519, 3520, 3521
  };
  static const uint16_t *prep_diff[PREP_FRAMES] = {
      prep_diff_0
  };

  static const uint16_t tap_diff_0[] PROGMEM = {
    187, 32841, 32842, 201, 202, 32971, 331, 33100, 33238, 33239, 33240, 33241, 588, 33357, 33366, 33367, 
    33368, 33369, 716, 33485, 33494, 33495, 33496, 33497, 33622, 33623, 33624, 33625, 33716, 33717, 33750, 33751, 
    33752, 33753, 33759, 33760, 33843, 1076, 1077, 33846, 33847, 33878, 33879, 33880, 33881, 33886, 33887, 33888, 
    33889, 33970, 1203, 1206, 1207, 34006, 34007, 34008, 34013, 34014, 34015, 34016, 34017, 34135, 34136, 34140, 
    34141, 34142, 34143, 34144, 34145, 34146, 34264, 34267, 34268, 34269, 34270, 34271, 34272, 34273, 34274, 34395, 
    34396, 34397, 34398, 34399, 34400, 34523, 34524, 1870, 1871, 1872, 1873, 34746, 34747, 34748, 1998, 1999, 
    2000, 2001, 34873, 34875, 34877, 2135, 35000, 35005, 35128, 35129, 2365, 2367, 35162, 35163, 35164, 35165, 
    35166, 35167, 35168, 35169, 35170, 35255, 35258, 35259, 2492, 2493, 2495, 35290, 35291, 35292, 35293, 35294, 
    35295, 35296, 35297, 35298, 35383, 35386, 35387, 35390, 35418, 35419, 35420, 35421, 35422, 35423, 35424, 35425, 
    35518, 35546, 35547, 35548, 35549, 35550, 35551, 35552, 35553, 35646, 35647, 35648, 35675, 35676, 35677, 35678, 
    35679, 35680, 35770, 35771, 35772, 35773, 3010, 35804, 35805, 35806, 35807, 35808, 3138, 35933, 35934, 35935, 
    3258, 3259, 3266, 36062, 36063, 3388, 3389, 3394, 3518, 3519, 3520, 3521
  };
  static const uint16_t tap_diff_1[] PROGMEM = {
    187, 32841, 32842, 201, 202, 32971, 331, 33100, 588, 33357, 716, 33485, 33716, 33717, 33843, 1076, 
    1077, 33846, 33847, 33875, 33876, 33877, 33878, 33970, 1203, 1206, 1207, 34002, 34007, 34133, 34136, 34257, 
    34263, 34264, 34385, 34389, 34392, 34512, 34516, 34520, 1870, 1871, 1873, 1878, 34648, 1998, 1999, 2001, 
    35027, 35028, 35029, 35030, 2264, 2364, 2365, 2366, 2367, 35152, 35153, 35154, 2392, 2492, 2493, 2494, 
    2495, 2512, 2513, 2520, 2642, 2643, 2644, 2645, 2646, 2647, 36274, 36275, 36276, 36277, 36278, 36279, 
    36280, 36402, 36403, 36404, 36405, 36406, 36407, 36408, 36409, 36530, 36531, 36532, 36533, 36534, 36535, 36536, 
    36537, 36658, 36659, 36660, 36661, 36662, 36663, 36800, 36801, 36802, 36803, 36922, 36923, 36924, 36928, 36929, 
    36930, 36931, 36932, 37049, 37050, 37051, 37052, 37056, 37057, 37058, 37059, 37060, 37061, 37176, 37177, 37178, 
    37179, 37180, 37184, 37185, 37186, 37187, 37188, 37189, 37190, 37303, 37304, 37305, 37306, 37307, 37312, 37313, 
    37314, 37315, 37316, 37317, 37318, 37319, 37430, 37431, 37432, 37433, 37434, 37435, 37440, 37441, 37442, 37443, 
    37444, 37445, 37446, 37447, 37558, 37559, 37560, 37561, 37562, 37563, 37568, 37569, 37570, 37571, 37572, 37573, 
    37687, 37688, 37689, 37690, 37696, 37697, 37698, 37699, 37817, 37818, 37824, 37825
  };
  static const uint16_t *tap_diff[TAP_FRAMES] = {
      tap_diff_0,
      tap_diff_1
  };

  /**
   * renders a PROGMEM uint16_t array, smaller or equal in size than the OLED
   * x, y: top left position of the image
   * width: width of the image
   */
  void render_short_array_pos(const uint16_t* frame, uint8_t x, uint8_t y, uint8_t width)
  {
    uint16_t size = pgm_read_word(&(frame[0])) + 1;
    for(uint16_t i=1; i<size; i++)
    {
      uint16_t cur_px = pgm_read_word(&(frame[i]));

      // get pixel state bit
      bool on = (cur_px & ( 1 << 15 )) >> 15;
      // remove pixel state bit
      cur_px &= ~(1UL << 15);

      uint8_t x_cur = (cur_px % width) + x;
      uint8_t y_cur = (cur_px / width) + y;
      oled_write_pixel(x_cur, y_cur, on);
    }
  }

  /**
   * Shortcut to render images of the size of the OLED
   */
  void render_short_array(const uint16_t* frame)
  {
    render_short_array_pos(frame, 0, 0, WIDTH);
  }

  //assumes 1 frame prep stage
  void animation_phase(void)
  {
    oled_clear();
    if (get_current_wpm() <= IDLE_SPEED)
    {
      current_idle_frame = (current_idle_frame + 1) % IDLE_FRAMES;
      // first render base frame
      render_short_array(base);
      // only add or remove changed pixels
      render_short_array(idle_diff[ current_idle_frame ]);
    }
    if (get_current_wpm() > IDLE_SPEED && get_current_wpm() < TAP_SPEED)
    {
      render_short_array(base);
      render_short_array(prep_diff[0]);
    }
    if (get_current_wpm() >= TAP_SPEED)
    {
      current_tap_frame = (current_tap_frame + 1) % TAP_FRAMES;
      render_short_array(base);
      render_short_array(tap_diff[ current_tap_frame ]);
    }
  }

  if(get_current_wpm() != 000)
  {
    oled_on(); // not essential but turns on animation OLED with any alpha keypress
    if(timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION)
    {
      anim_timer = timer_read32();
      animation_phase();
    }
    anim_sleep = timer_read32();
  } else
  {
    if(timer_elapsed32(anim_sleep) > OLED_TIMEOUT)
    {
      oled_off();
    } else
    {
      if(timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION)
      {
        anim_timer = timer_read32();
        animation_phase();
      }
    }
  }
}

void render_wpm(void)
{
  char wpm_number_str[3];
  itoa(get_current_wpm(), wpm_number_str, 10);
  oled_set_cursor(7,6);
  strcpy(wpm_str, "WPM: ");
  strcat(wpm_str, wpm_number_str);
  oled_write(wpm_str, false);
}

#endif // BONGOCAT_ENABLE

#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
	return OLED_ROTATION_180;
}

static void render_status(void) {
    // Host Keyboard Layer Status
    oled_set_cursor(0, 6);
    oled_write_P(PSTR("Layer: "), false);
    switch (get_highest_layer(layer_state)) {
        case _QWERTY:
             oled_write_P(PSTR("Default\n"), false);
            break;
        case _LOWER:
            oled_write_P(PSTR("Lower\n"), false);
            break;
        case _RAISE:
            oled_write_P(PSTR("Raise\n"), false);
            break;
        case _ADJUST:
            oled_write_P(PSTR("Adjust\n"), false);
            break;
        default:
            oled_write_P(PSTR("Undefined\n"), false);
    }

    // Host Keyboard LED Status
    uint8_t led_usb_state = host_keyboard_leds();
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_NUM_LOCK) ? PSTR("NUM  ") : PSTR("     "), false);
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_CAPS_LOCK) ? PSTR("CAPS ") : PSTR("     "), false);
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_SCROLL_LOCK) ? PSTR("SCRL ") : PSTR("     "), false);

#ifdef INFO_OLED_ENABLE
    if (is_hid_connected) {
      oled_write_P(PSTR("   ["), false);

      // Convert int to char by ascii value
      char page;
      if (screen_show_index < 10) {
        itoa(screen_show_index, &page, 10);
      } else {
        page = '*';
      }
      
      oled_write(&page, false);

      oled_write_P(PSTR("]"), false);
    }
#endif
}

static void render_magic_logo(void) {
  static const char PROGMEM qmk_logo[] = {
    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
    0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
    0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0};

  oled_write_P(qmk_logo, false);
}

void oled_task_user(void) {
  if (is_keyboard_master()) {
  #ifdef INFO_OLED_ENABLE
    // First 4 lines are either written from device or info panel
    if (screen_show_index == 0) {
      // Logo and version information
      render_magic_logo();

      oled_write_P(PSTR("Kyria r1.0 @ Hegezcc \n\n"), false);
    } else {
      render_info_panel();
    }
  #else
    render_info_panel();
  #endif

    // We also render the current keyboard state (layer, lock, caps, scroll, etc)
    render_status();
  } else {
  #ifdef BONGOCAT_ENABLE
    render_anim();
    render_wpm();
  #endif
  }
}
#endif

#ifdef ENCODER_ENABLE
void encoder_update_user(uint8_t index, bool clockwise) {
  if (index == 0) {
    // Volume control
    if (clockwise) {
      tap_code(KC_VOLD);
    } else {
      tap_code(KC_VOLU);
    }
  } else if (index == 1) {
    switch (biton32(layer_state)) {
      case _ADJUST: {
        #ifdef INFO_OLED_ENABLE
        if (!is_hid_connected) return;
        #endif

        // On the RGB layer we control the screen display with the encoder
        if (clockwise) {
          // Decrement and loop back to the end if we are about to go below zero,
          // Be careful since index is unsigned.
          if (screen_show_index == 0) {
            screen_show_index = screen_max_count - 1;
          } else {
            screen_show_index--;
          }
        } else {
          // Increment and loop back to beginning if we go over the max
          screen_show_index++;
          if (screen_show_index >= screen_max_count) {
            screen_show_index = 0;
          }
        }

        #ifdef INFO_OLED_ENABLE
        // If we have a connection we should tell it about the change,
        // Otherwise it will be notified when it first connects instead.
        if (is_hid_connected) {
          raw_hid_send_screen_index();
        }
        #endif
        break;
      }

      case _RAISE: {
        // Scroll horizontally
        for (int i = 0; i < ENCODER_SCROLL_COUNT; ++i) {
          if (clockwise) {
            tap_code(KC_MS_WH_LEFT);
          } else {
            tap_code(KC_MS_WH_RIGHT);
          }
        }

        break;
      }

      default: {
        // Scroll vertically
        for (int i = 0; i < ENCODER_SCROLL_COUNT; ++i) {
          if (clockwise) {
            tap_code(KC_MS_WH_DOWN);
          } else {
            tap_code(KC_MS_WH_UP);
          }
        }
        break;
      }
    }
  }
}
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case MOUSE_BTN: // Tap-hold mouse button: BTN1 on short tap, BTN2 on long hold
      if (record->event.pressed) {
        mouse_button_timer = timer_read();
      } else {
        if (timer_elapsed(mouse_button_timer) > TAPHOLD_TIME) {
          tap_code(KC_BTN2);
        } else {
          tap_code(KC_BTN1);
        }
      }
      return false;
    default:
      return true;
  }
}