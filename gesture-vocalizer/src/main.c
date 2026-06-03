/**
 * Wearable Gesture Vocalizer System
 * ESP32 + Flex Sensors + DFPlayer Mini
 *
 * Detects predefined hand gestures via ADC and triggers
 * corresponding voice outputs through DFPlayer Mini over UART.
 */

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "gesture_recognition.h"
#include "dfplayer.h"

static const char *TAG = "GestureVocalizer";

// ── Task: Read sensors & recognize gestures ───────────────────────────────────
void gesture_task(void *pvParameters) {
    sensor_data_t sensor_data;
    gesture_id_t  gesture;

    while (1) {
        // Acquire raw ADC values from all flex sensors
        sensor_data_acquire(&sensor_data);

        // Threshold-driven gesture classification
        gesture = gesture_recognize(&sensor_data);

        if (gesture != GESTURE_NONE) {
            ESP_LOGI(TAG, "Gesture detected: %d", gesture);
            dfplayer_play(gesture);              // play track matching gesture ID
            vTaskDelay(pdMS_TO_TICKS(1000));     // debounce delay
        }

        vTaskDelay(pdMS_TO_TICKS(50));           // 20 Hz polling
    }
}

void app_main(void) {
    ESP_LOGI(TAG, "Wearable Gesture Vocalizer – Initialising");

    // Initialise subsystems
    sensor_init();
    dfplayer_init();

    ESP_LOGI(TAG, "All subsystems ready. Starting gesture task.");
    xTaskCreate(gesture_task, "gesture_task", 4096, NULL, 5, NULL);
}
