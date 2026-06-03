#include "dfplayer.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "DFPlayer";

/**
 * Build and send a 10-byte DFPlayer Mini command frame.
 *
 * Frame format: 7E FF 06 CMD FB HH LL CS_H CS_L EF
 * CS = 0xFFFF - (VER + LEN + CMD + FB + HH + LL) + 1
 */
static void dfplayer_send_cmd(uint8_t cmd, uint8_t param_hi, uint8_t param_lo) {
    uint16_t checksum = 0xFFFF
        - (DF_VERSION + 0x06 + cmd + 0x00 + param_hi + param_lo) + 1;

    uint8_t frame[10] = {
        DF_START_BYTE,
        DF_VERSION,
        0x06,               // data length (always 6)
        cmd,
        0x00,               // feedback: 0 = no ACK
        param_hi,
        param_lo,
        (uint8_t)(checksum >> 8),
        (uint8_t)(checksum & 0xFF),
        DF_END_BYTE
    };

    uart_write_bytes(DFPLAYER_UART_NUM, (const char *)frame, sizeof(frame));
    ESP_LOGD(TAG, "Sent cmd=0x%02X param=%02X%02X", cmd, param_hi, param_lo);
}

void dfplayer_init(void) {
    uart_config_t cfg = {
        .baud_rate  = DFPLAYER_BAUD_RATE,
        .data_bits  = UART_DATA_8_BITS,
        .parity     = UART_PARITY_DISABLE,
        .stop_bits  = UART_STOP_BITS_1,
        .flow_ctrl  = UART_HW_FLOWCTRL_DISABLE,
    };
    uart_param_config(DFPLAYER_UART_NUM, &cfg);
    uart_set_pin(DFPLAYER_UART_NUM, DFPLAYER_TX_PIN, DFPLAYER_RX_PIN,
                 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(DFPLAYER_UART_NUM, 256, 0, 0, NULL, 0);

    vTaskDelay(pdMS_TO_TICKS(500));          // allow module to boot
    dfplayer_set_volume(DF_DEFAULT_VOL);
    ESP_LOGI(TAG, "DFPlayer Mini ready on UART%d", DFPLAYER_UART_NUM);
}

void dfplayer_play(uint8_t track) {
    ESP_LOGI(TAG, "Playing track %d", track);
    dfplayer_send_cmd(DF_CMD_PLAY, 0x00, track);
}

void dfplayer_set_volume(uint8_t volume) {
    if (volume > 30) volume = 30;
    dfplayer_send_cmd(DF_CMD_VOLUME, 0x00, volume);
}
