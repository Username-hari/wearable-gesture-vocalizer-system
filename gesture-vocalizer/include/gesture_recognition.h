#pragma once

#include <stdint.h>

// ── Sensor configuration ──────────────────────────────────────────────────────
#define NUM_FLEX_SENSORS    5       // one per finger
#define ADC_MAX_VALUE       4095    // 12-bit ADC

// Thresholds (tune after physical calibration)
#define FLEX_THRESHOLD_LOW  1200
#define FLEX_THRESHOLD_HIGH 2800

// ── Data types ────────────────────────────────────────────────────────────────
typedef struct {
    uint16_t raw[NUM_FLEX_SENSORS];  // raw ADC readings (0–4095)
    uint8_t  bent[NUM_FLEX_SENSORS]; // 1 = finger bent, 0 = straight
} sensor_data_t;

typedef enum {
    GESTURE_NONE = 0,
    GESTURE_HELLO,        // track 01 – "Hello"
    GESTURE_THANKYOU,     // track 02 – "Thank you"
    GESTURE_YES,          // track 03 – "Yes"
    GESTURE_NO,           // track 04 – "No"
    GESTURE_HELP,         // track 05 – "Help"
    GESTURE_WATER,        // track 06 – "Water"
    GESTURE_COUNT
} gesture_id_t;

// ── Public API ────────────────────────────────────────────────────────────────
void       sensor_init(void);
void       sensor_data_acquire(sensor_data_t *out);
gesture_id_t gesture_recognize(const sensor_data_t *data);
