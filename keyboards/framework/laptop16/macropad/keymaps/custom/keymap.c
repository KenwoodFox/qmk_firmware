// Written by Joe
// Aided by the Framework Community https://community.frame.work/t/custom-qmk-firmware-for-macropad/51466
// Adapted from Copyright work of 2022 Framework Computer
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "factory.h"
#include <lib/lib8tion/lib8tion.h>


/* Flashing and Editing Instructions:
 * To put the macropad in firmware mode, disconnect module. Reinstall while pressing 2 and 6
 * (Row 5 col 2, row 4 col 3)
 * Then `qmk flash -kb framework/laptop16/macropad -km custom`
 */

enum custom_keycodes {
    WORDCOUNT = SAFE_RANGE,
};

// My defs/settings definition
// #define CLEAR_EEPROM_ON_BOOT
#define MIN_BRIGHTNESS 4
#define RGB_LIGHT_BLUE 100, 100, 255

// Helper function to set RGB color scaled by current brightness
static void rgb_matrix_set_color_scaled(int index, uint8_t r, uint8_t g, uint8_t b) {
#ifdef RGB_MATRIX_ENABLE
    uint8_t brightness = rgb_matrix_get_val() < MIN_BRIGHTNESS ? MIN_BRIGHTNESS : rgb_matrix_get_val();
    rgb_matrix_set_color(index, 
        scale8(r, brightness),
        scale8(g, brightness),
        scale8(b, brightness));
#endif
}

void keyboard_post_init_user(void) {
#ifdef CLEAR_EEPROM_ON_BOOT
    eeconfig_init();  // This clears EEPROM
#endif
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(   // Numpad
        TO(0),         TO(1),         TO(2),             TO(3),
        KC_PRINT_SCREEN,       KC_KP_SLASH,   KC_KP_ASTERISK,    KC_KP_MINUS,
        KC_KP_7,       KC_KP_8,       KC_KP_9,           KC_KP_PLUS,
        KC_KP_4,       KC_KP_5,       KC_KP_6,           KC_PERCENT,
        KC_KP_1,       KC_KP_2,       KC_KP_3,           KC_KP_EQUAL,
        KC_BACKSPACE,  KC_KP_0,       KC_KP_DOT,         KC_ENTER
    ),
    [1] = LAYOUT(   // Nav
        _______,       _______,       _______,            _______,
        XXXXXXX,       _______,       KC_VOLD,            KC_VOLU,
        XXXXXXX,       XXXXXXX,       XXXXXXX,            XXXXXXX,
        KC_HOME,       KC_PGUP,       KC_PGDN,            KC_END,
        XXXXXXX,       KC_UP,         XXXXXXX,            XXXXXXX,
        KC_LEFT,       KC_DOWN,       KC_RIGHT,           _______
    ),
    [2] = LAYOUT(   // Document/Chat
        _______,       _______,       _______,            _______,
        C(KC_B),       C(KC_I),       C(KC_U),            LSA(KC_5),
        RCS(KC_L),     RCS(KC_E),     RCS(KC_R),          WORDCOUNT,
        LCA(KC_4),     LCA(KC_5),     XXXXXXX,            RCS(KC_8),
        LCA(KC_1),     LCA(KC_2),     LCA(KC_3),          RCS(KC_7),
        XXXXXXX,       LCA(KC_0),     XXXXXXX,            _______
    ),
    [3] = LAYOUT(   // Special
        _______,       _______,       _______,            _______,
        RGB_VAI,       XXXXXXX,       XXXXXXX,            XXXXXXX,
        XXXXXXX,       XXXXXXX,       RGB_VAD,            RGB_VAI,
        XXXXXXX,       XXXXXXX,       XXXXXXX,            XXXXXXX,
        QK_BOOT,       XXXXXXX,       XXXXXXX,            XXXXXXX,
        XXXXXXX,       XXXXXXX,       XXXXXXX,            _______
    ),

};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case WORDCOUNT:
      if (record->event.pressed) {
        tap_code16(RCS(KC_C));
        wait_ms(250);
        tap_code16(KC_SPACE);
        wait_ms(50);
        tap_code16(KC_TAB);
        wait_ms(50);
        tap_code16(KC_TAB);
        wait_ms(50);
        tap_code16(KC_ENTER);
      }
      return false;
  }
  return true;
}

bool rgb_matrix_indicators_user(void) {
    // Default 1hz flash (for any key to use)
    uint32_t current_time = timer_read32();
    bool flash_on = (current_time / 1000) % 2 == 0;

    if (layer_state_cmp(layer_state, 0)) {
        // Numpad 
        rgb_matrix_set_color_scaled(4,  RGB_WHITE);    // RGB      C1 R2
        rgb_matrix_set_color_scaled(7,  RGB_RED);      // 7 	    C1 R3
        rgb_matrix_set_color_scaled(6,  RGB_RED);      // 4        C1 R4
        rgb_matrix_set_color_scaled(9,  RGB_RED);      // 1 	    C1 R5
        rgb_matrix_set_color_scaled(0,  RGB_PINK);     // slash    C2 R2
        rgb_matrix_set_color_scaled(1,  RGB_RED);      // 8    	C2 R3
        rgb_matrix_set_color_scaled(3,  RGB_RED);      // 5    	C2 R4
        rgb_matrix_set_color_scaled(11, RGB_RED);      // 2 		C2 R5
        rgb_matrix_set_color_scaled(8,  RGB_PINK);     // BkSp	    C1 R6
        rgb_matrix_set_color_scaled(20, RGB_PINK);     // X 		C3 R2
        rgb_matrix_set_color_scaled(21, RGB_RED);      // 9		C3 R3
        rgb_matrix_set_color_scaled(23, RGB_RED);      // 6		C3 R4
        rgb_matrix_set_color_scaled(15, RGB_RED);      // 3		C3 R5
        rgb_matrix_set_color_scaled(14, RGB_PINK);     // .		C3 R6 
        rgb_matrix_set_color_scaled(18, RGB_PINK);     // -	    C4 R2
        rgb_matrix_set_color_scaled(19, RGB_PINK);     // %		C4 R4 
        rgb_matrix_set_color_scaled(12, RGB_WHITE);    // Enter    C4 R6
        {
            if (flash_on) {
                rgb_matrix_set_color_scaled(5,  RGB_RED);      // Numpad   C1 R1
            } else {
                rgb_matrix_set_color_scaled(5,  RGB_BLACK);      // Numpad   C1 R1
            }
        }
        rgb_matrix_set_color_scaled(2,  RGB_GREEN);    // Arrows   C2 R1
        rgb_matrix_set_color_scaled(17, RGB_YELLOW);   // Document/Chat	C4 R1
        rgb_matrix_set_color_scaled(16, RGB_PINK);     // +		C4 R3
        rgb_matrix_set_color_scaled(13, RGB_PINK);     // =		C4 R5
        rgb_matrix_set_color_scaled(10, RGB_RED);      // 0		C2 R6
        rgb_matrix_set_color_scaled(22, RGB_BLUE);     // Docs     C3 R1
    }
    if (layer_state_cmp(layer_state, 1)) {
        // Navigation 
        rgb_matrix_set_color_scaled(4,  RGB_WHITE);        // RGB      C1 R2
        rgb_matrix_set_color_scaled(7,  RGB_BLACK);        // None 	C1 R3
        rgb_matrix_set_color_scaled(6,  RGB_SPRINGGREEN);  // Home     C1 R4
        rgb_matrix_set_color_scaled(9,  RGB_BLACK);        // None 	C1 R5
        rgb_matrix_set_color_scaled(0,  RGB_BLACK);        // None    	C2 R2
        rgb_matrix_set_color_scaled(1,  RGB_BLACK);        // None    	C2 R3
        rgb_matrix_set_color_scaled(3,  RGB_SPRINGGREEN);  // Page Up	C2 R4
        rgb_matrix_set_color_scaled(11, RGB_GREEN);        // Up 		C2 R5
        rgb_matrix_set_color_scaled(8,  RGB_GREEN);        // Left	    C1 R6
        rgb_matrix_set_color_scaled(20, RGB_SPRINGGREEN);  // ScrLck   C3 R2
        rgb_matrix_set_color_scaled(21, RGB_BLACK);        // None		C3 R3
        rgb_matrix_set_color_scaled(23, RGB_SPRINGGREEN);  // Page Dwn	C3 R4
        rgb_matrix_set_color_scaled(15, RGB_BLACK);        // None		C3 R5
        rgb_matrix_set_color_scaled(14, RGB_GREEN);        // Right	C3 R6 
        rgb_matrix_set_color_scaled(18, RGB_SPRINGGREEN);  // Prt Scr  C4 R2
        rgb_matrix_set_color_scaled(19, RGB_SPRINGGREEN);  // End		C4 R4 
        rgb_matrix_set_color_scaled(12, RGB_WHITE);        // Enter    C4 R6
        rgb_matrix_set_color_scaled(5,  RGB_RED);          // Numpad   C1 R1
        {
            if (flash_on) {
                rgb_matrix_set_color_scaled(2,  RGB_GREEN);        // Arrows   C2 R1
            } else {
                rgb_matrix_set_color_scaled(2,  RGB_BLACK);        // Arrows   C2 R1
            }
        }
        rgb_matrix_set_color_scaled(17, RGB_YELLOW);       // Document/Chat	C4 R1
        rgb_matrix_set_color_scaled(16, RGB_BLACK);        // None		C4 R3
        rgb_matrix_set_color_scaled(13, RGB_BLACK);        // None		C4 R5
        rgb_matrix_set_color_scaled(10, RGB_GREEN);        // Down		C2 R6
        rgb_matrix_set_color_scaled(22, RGB_BLUE);         // Docs     C3 R1
    }
    if (layer_state_cmp(layer_state, 2)) {
        // Document/Chat 
        rgb_matrix_set_color_scaled(4,  RGB_BLUE);    // Bold      C1 R2
        rgb_matrix_set_color_scaled(7,  RGB_TEAL);    // Left	    C1 R3
        rgb_matrix_set_color_scaled(6,  RGB_BLUE);    // Head 4    C1 R4
        rgb_matrix_set_color_scaled(9,  RGB_BLUE);    // Head 1    C1 R5
        rgb_matrix_set_color_scaled(0,  RGB_BLUE);    // italic    C2 R2
        rgb_matrix_set_color_scaled(1,  RGB_TEAL);    // Center 	C2 R3
        rgb_matrix_set_color_scaled(3,  RGB_BLUE);    // Head 5 	C2 R4
        rgb_matrix_set_color_scaled(11, RGB_BLUE);    // Head 2	C2 R5
        rgb_matrix_set_color_scaled(8,  RGB_BLACK);   // None	    C1 R6
        rgb_matrix_set_color_scaled(20, RGB_BLUE);    // U Line	C3 R2
        rgb_matrix_set_color_scaled(21, RGB_TEAL);    // Right     C3 R3
        rgb_matrix_set_color_scaled(23, RGB_BLACK);   // None		C3 R4
        rgb_matrix_set_color_scaled(15, RGB_BLUE);    // Head 3    C3 R5
        rgb_matrix_set_color_scaled(14, RGB_BLACK);   // None		C3 R6 
        rgb_matrix_set_color_scaled(18, RGB_BLUE);    // Stike     C4 R2
        rgb_matrix_set_color_scaled(19, RGB_TEAL);    // bullet	C4 R4 
        rgb_matrix_set_color_scaled(12, RGB_WHITE);   // Enter     C4 R6
        rgb_matrix_set_color_scaled(5,  RGB_RED);     // Numpad    C1 R1
        rgb_matrix_set_color_scaled(2,  RGB_GREEN);   // Arrows    C2 R1
        rgb_matrix_set_color_scaled(17, RGB_YELLOW);  // Document/Chat  C4 R1
        rgb_matrix_set_color_scaled(16, RGB_TEAL);    // WordCount	C4 R3
        rgb_matrix_set_color_scaled(13, RGB_TEAL);    // numbers	C4 R5
        rgb_matrix_set_color_scaled(10, RGB_BLUE);    // normal	C2 R6
        {
            if (flash_on) {
                rgb_matrix_set_color_scaled(22, RGB_BLUE);         // Docs     C3 R1
            } else {
                rgb_matrix_set_color_scaled(22, RGB_BLACK);         // Docs     C3 R1
            }
        }
    }
    if (layer_state_cmp(layer_state, 3)) {
        // Special 
        rgb_matrix_set_color_scaled(4,  RGB_WHITE);        // RGB      C1 R2
        rgb_matrix_set_color_scaled(7,  RGB_BLACK);        //          C1 R3
        rgb_matrix_set_color_scaled(6,  RGB_BLACK);        //   C1 R4
        // Special blinky flash
        {
            bool _flash_on = (current_time / 200) % 2 == 0;  // 5Hz: on for 200ms, off for 200ms
            if (_flash_on) {
                rgb_matrix_set_color_scaled(9, RGB_RED);    // Flash!  C1 R5
            } else {
                rgb_matrix_set_color_scaled(9, RGB_BLACK);  // Flash off
            }
        }
        rgb_matrix_set_color_scaled(0,  RGB_BLACK);        //    C2 R2
        rgb_matrix_set_color_scaled(1,  RGB_BLACK);        //      C2 R3
        rgb_matrix_set_color_scaled(3,  RGB_BLACK);        //     C2 R4
        rgb_matrix_set_color_scaled(11, RGB_BLACK);        //     C2 R5
        rgb_matrix_set_color_scaled(8,  RGB_BLACK);        //   C1 R6
        rgb_matrix_set_color_scaled(20, RGB_BLACK);        //  C3 R2
        rgb_matrix_set_color_scaled(21, RGB_LIGHT_BLUE);         // BR-     C3 R3
        rgb_matrix_set_color_scaled(23, RGB_BLACK);        //     C3 R4
        rgb_matrix_set_color_scaled(15, RGB_BLACK);        //     C3 R5
        rgb_matrix_set_color_scaled(14, RGB_BLACK);        //  C3 R6
        rgb_matrix_set_color_scaled(18, RGB_BLACK);        //   C4 R2
        rgb_matrix_set_color_scaled(19, RGB_BLACK);        //      C4 R4 
        rgb_matrix_set_color_scaled(12, RGB_BLACK);        //     C4 R6
        rgb_matrix_set_color_scaled(5,  RGB_RED);          // Numpad   C1 R1
        rgb_matrix_set_color_scaled(2,  RGB_GREEN);        // Arrows   C2 R1
        {
            if (flash_on) {
                rgb_matrix_set_color_scaled(17, RGB_YELLOW);       // Document/Chat	C4 R1
            } else {
                rgb_matrix_set_color_scaled(17, RGB_BLACK);       // Document/Chat	C4 R1
            }
        }
        rgb_matrix_set_color_scaled(16, RGB_BLUE);         // BR+      C4 R3
        rgb_matrix_set_color_scaled(13, RGB_BLACK);        //      C4 R5
        rgb_matrix_set_color_scaled(10, RGB_BLACK);        //     C2 R6
        rgb_matrix_set_color_scaled(22, RGB_BLUE);         // Docs     C3 R1
        rgb_matrix_set_color_scaled(22, RGB_BLUE);    // Docs      C3 R1
    }
    return true;
}
