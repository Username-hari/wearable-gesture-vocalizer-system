#include "gesture_recognition.h"
#include "driver/adc.h"
#include "esp_log.h"

static const char *TAG = "GestureRecog";

// GPIO / ADC channel mapping  (adjust to your wiring)
static const adc1_channel_t FLEX_CHANNELS[NUM_FLEX_SENSORS] = {
    ADC1_CHANNEL_0,  // Thumb   – GPIO36
    ADC1_CHANNEL_3,  // Index   – GPIO39
    ADC1_CHANNEL_6,  // Middle  – GPIO34
    ADC1_CHANNEL_7,  // Ring    – GPIO35
    ADC1_CHANNEL_4,  // Pinky   – GPIO32
};

/**
 * Gesture look-up table.
 * Each row = bent-finger pattern for one gesture.
 * 1 = bent, 0 = straight, 2 = don't-care.
 *
 * Order: [Thumb, Index, Middle, Ring, Pinky]
 */
static const uint8_t GESTURE_PATTERNS[GESTURE_COUNT][NUM_FLEX_SENSORS] = {
    [GESTURE_NONE]     = {2, 2, 2, 2, 2},
    [GESTURE_HELLO]    = {0, 0, 0, 0, 0},  // open palm
    [GESTURE_THANKYOU] = {0, 1, 1, 1, 1},  // thumb up
    [GESTURE_YES]      = {1, 1, 1, 1, 1},  // closed fist
    [GESTURE_NO]       = {0, 1, 0, 0, 0},  // index point
    [GESTURE_HELP]     = {0, 1, 1, 0, 0},  // peace / V
    [GESTURE_WATER]    = {1, 0, 1, 1, 1},  // W shape
};

void sensor_init(void) {
    adc1_config_width(ADC_WIDTH_BIT_12);
    for (int i = 0; i < NUM_FLEX_SENSORS; i++) {
        adc1_config_channel_atten(FLEX_CHANNELS[i], ADC_ATTEN_DB_11);
    }
    ESP_LOGI(TAG, "ADC initialised for %d flex sensors", NUM_FLEX_SENSORS);
}

void sensor_data_acquire(sensor_data_t *out) {
    for (int i = 0; i < NUM_FLEX_SENSORS; i++) {
        out->raw[i]  = (uint16_t)adc1_get_raw(FLEX_CHANNELS[i]);
        out->bent[i] = (out->raw[i] > FLEX_THRESHOLD_HIGH) ? 1 : 0;
    }
}

gesture_id_t gesture_recognize(const sensor_data_t *data) {
    for (gesture_id_t g = GESTURE_HELLO; g < GESTURE_COUNT; g++) {
        int match = 1;
        for (int f = 0; f < NUM_FLEX_SENSORS; f++) {
            if (GESTURE_PATTERNS[g][f] == 2) continue;          // don't-care
            if (GESTURE_PATTERNS[g][f] != data->bent[f]) {
                match = 0;
                break;
            }
        }
        if (match) return g;
    }
    return GESTURE_NONE;
}
