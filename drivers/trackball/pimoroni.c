#include "pimoroni.h"

#include "i2c_master.h"
#include "rgblight.h"
#ifdef POINTING_DEVICE_ENABLE
    #include "pointing_device.h"
#endif
#include "matrix.h"
#include "keyboard.h"

// See https://github.com/pimoroni/trackball-python/blob/master/library/trackball/__init__.py

#define LED_REG 0x00
#define INPUT_REG 0x04
#define INTERRUPT_REG 0xF9

#define MSK_BTN_STATE  0b10000000
#define MSK_BTN_CHANGE 0b00000001
#define MSK_INT_TRIGGER 0b00000001

typedef struct {
    uint8_t left;
    uint8_t right;
    uint8_t up;
    uint8_t down;
    // bit 1: button changed
    // bit 8: button state
    uint8_t button;
} input_t;

// Defined in `quantum/matrix_common.c`
extern matrix_row_t matrix[MATRIX_ROWS];

#ifdef TRACKBALL_RGBLIGHT
// Defined in `quantum/rgblight.c`
extern LED_TYPE led[RGBLED_NUM];
#endif

void trackball_init(void) {
    if (!is_keyboard_master()) return;
    i2c_init();

    // The trackball may already have some stale data, so clear its registers
    input_t input;
    i2c_readReg(TRACKBALL_ADDRESS << 1, INPUT_REG, (uint8_t*)&input, sizeof(input), TRACKBALL_TIMEOUT);

    // Clear the LED
    trackball_setrgb(0, 0, 0);
}

void trackball_task(void) {
    i2c_status_t status;

#ifdef TRACKBALL_RGBLIGHT
    trackball_setrgb(led[TRACKBALL_RGBLIGHT].r, led[TRACKBALL_RGBLIGHT].g, led[TRACKBALL_RGBLIGHT].b);
#endif

    uint8_t interrupt;
    status = i2c_readReg(TRACKBALL_ADDRESS << 1, INTERRUPT_REG, &interrupt, sizeof(interrupt), TRACKBALL_TIMEOUT);
    if (status != I2C_STATUS_SUCCESS || !(interrupt & MSK_INT_TRIGGER)) {
        // Interrupt is not triggered, so there is no data to read
        return;
    }

    input_t input;
    status = i2c_readReg(TRACKBALL_ADDRESS << 1, INPUT_REG, (uint8_t*)&input, sizeof(input), TRACKBALL_TIMEOUT);
    if (status != I2C_STATUS_SUCCESS) {
        return;
    }

    trackball_record_t record = { .type = 0 };

#if TRACKBALL_ORIENTATION == 0
    // Pimoroni text is pointing up
    record.x += input.right - input.left;
    record.y += input.down - input.up;
#elif TRACKBALL_ORIENTATION == 1
    // Pimoroni text is pointing right
    record.x += input.up - input.down;
    record.y += input.right - input.left;
#elif TRACKBALL_ORIENTATION == 2
    // Pimoroni text is pointing down
    record.x += input.left - input.right;
    record.y += input.up - input.down;
#else
    // Pimoroni text is pointing left
    record.x += input.down - input.up;
    record.y += input.left - input.right;
#endif
    if (record.x != 0 || record.y != 0) {
        record.type |= TB_MOVED;
    }

    record.pressed = input.button & MSK_BTN_STATE;
    if (input.button & MSK_BTN_CHANGE) {
        record.type |= TB_BUTTON;
    }

    process_trackball_kb(&record);

#if defined(TRACKBALL_MATRIX_ROW) && defined(TRACKBALL_MATRIX_COL)
    if (record.type & TB_BUTTON) {
        // The trackball is used as a regular key in the matrix
        matrix[TRACKBALL_MATRIX_ROW] &= ~(MATRIX_ROW_SHIFTER << TRACKBALL_MATRIX_COL);
        matrix[TRACKBALL_MATRIX_ROW] |= record.pressed ? (MATRIX_ROW_SHIFTER << TRACKBALL_MATRIX_COL) : 0;
        record.type &= ~TB_BUTTON;
    }
#endif

#ifdef POINTING_DEVICE_ENABLE
    report_mouse_t currentReport = pointing_device_get_report();
    bool send_report = false;

    if (record.type & TB_BUTTON) {
        send_report = true;
        if (record.pressed) {
            currentReport.buttons |= TRACKBALL_MOUSE_BTN;
        } else {
            currentReport.buttons &= ~TRACKBALL_MOUSE_BTN;
        }
    }

    if (record.type & TB_MOVED) {
        send_report = true;

        #define SIGN(x) ((x > 0) - (x < 0))
        uint8_t scale = 3;
        //if (mods & MOD_MASK_CTRL) scale = 2;
        currentReport.x += record.x * record.x * SIGN(record.x) * scale;
        currentReport.y += record.y * record.y * SIGN(record.y) * scale;
    }

    if (send_report) {
        pointing_device_set_report(currentReport);
    }
#endif
}

i2c_status_t trackball_setrgb(uint8_t r, uint8_t g, uint8_t b) {
    // Trackball supports RGBW, but we just ignore the W
    uint8_t led_buf[] = { r, g, b, 0 };
    return i2c_writeReg(TRACKBALL_ADDRESS << 1, LED_REG, led_buf, sizeof(led_buf), TRACKBALL_TIMEOUT);
}

i2c_status_t trackball_sethsv(uint8_t h, uint8_t s, uint8_t v) {
    RGB out = hsv_to_rgb((HSV){.h = h, .s = s, .v = v});
    return trackball_setrgb(out.r, out.g, out.b);
}

__attribute__((weak)) void process_trackball_kb(trackball_record_t *record) { return process_trackball_user(record); }
__attribute__((weak)) void process_trackball_user(trackball_record_t *record) { return; }
