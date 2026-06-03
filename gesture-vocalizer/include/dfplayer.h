#pragma once

#include <stdint.h>

// ── UART configuration ────────────────────────────────────────────────────────
#define DFPLAYER_UART_NUM   UART_NUM_1
#define DFPLAYER_TX_PIN     17
#define DFPLAYER_RX_PIN     16
#define DFPLAYER_BAUD_RATE  9600

// ── DFPlayer Mini frame constants ─────────────────────────────────────────────
#define DF_START_BYTE   0x7E
#define DF_VERSION      0xFF
#define DF_CMD_PLAY     0x03
#define DF_CMD_VOLUME   0x06
#define DF_DEFAULT_VOL  25      // 0–30
#define DF_END_BYTE     0xEF

// ── Public API ────────────────────────────────────────────────────────────────
void dfplayer_init(void);
void dfplayer_play(uint8_t track);
void dfplayer_set_volume(uint8_t volume);
