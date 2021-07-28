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

#pragma once

#ifdef OLED_DRIVER_ENABLE
  #define OLED_DISPLAY_128X64
  #define OLED_FONT_H "keyboards/kyria/keymaps/Hegezcc/glcdfont.c"
#endif

#ifdef RGBLIGHT_ENABLE
  #define RGBLIGHT_EFFECT_STATIC_GRADIENT
  #define RGBLIGHT_EFFECT_RAINBOW_SWIRL
  #define RGBLIGHT_HUE_STEP 16
  #define RGBLIGHT_SAT_STEP 16
  #define RGBLIGHT_VAL_STEP 16
  #define RGBLIGHT_LIMIT_VAL 150
#endif

// Right side as master
#define MASTER_RIGHT

// Trackball
#define TRACKBALL_MATRIX_ROW 6
#define TRACKBALL_MATRIX_COL 1
#define TRACKBALL_ORIENTATION 2
#ifdef RGBLIGHT_ENABLE
	#define TRACKBALL_RGBLIGHT 11
  #define TRACKBALL_RGB_ACTION 255, 0, 0
#endif

#ifdef ENCODER_ENABLE
  // Rotary encoder resolution fix
  #define ENCODER_RESOLUTION 2

  // Rotary encoder scroll amount
  #define ENCODER_SCROLL_COUNT 5
#endif

// Custom tap-hold handler
#define TAPHOLD_TIME 200

// OLED info script
#define INFO_OLED_ENABLE
#define INFO_LINE_COUNT 6   // Should correspond to OLED line count allowed to program (max height - 2)
#define INFO_LINE_LENGTH 21 // Should correspond to OLED char width
#define INFO_BUFFER_SIZE (INFO_LINE_COUNT * INFO_LINE_LENGTH)

#ifdef WPM_ENABLE
  #define BONGOCAT_ENABLE
#endif

#ifdef BONGOCAT_ENABLE
// From https://github.com/Dakes/kyria/blob/main/keymaps/dakes/keymap.c
// originally by j-inc: https://github.com/qmk/qmk_firmware/blob/master/keyboards/kyria/keymaps/j-inc/keymap.c
// Adapted to modular system and diffs using uint16_t arrays by Dakes (Daniel Ostertag).

  // WPM-responsive animation stuff here
  #define IDLE_FRAMES 5 // 5
  #define IDLE_SPEED 20 // below this wpm value your animation will idle

  #define PREP_FRAMES 1 // uncomment if >1

  #define TAP_FRAMES 2
  #define TAP_SPEED 40 // above this wpm value typing animation to triggered

  #define PREP_FRAMES 1

  #define ANIM_FRAME_DURATION 200 // how long each frame lasts in ms 200
  // #define SLEEP_TIMER 60000 // should sleep after this period of 0 wpm, needs fixing

  // OLED width
  #define WIDTH 128
#endif // BONGOCAT_ENABLE

//#define UNICODE_SELECTED_MODES UC_WINC

#define TAPPING_TERM 200
#define IGNORE_MOD_TAP_INTERRUPT
#define TAPPING_FORCE_HOLD
#define PERMISSIVE_HOLD
