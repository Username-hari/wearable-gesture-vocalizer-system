# 🤟 Wearable Gesture Vocalizer System

A wearable assistive communication prototype that detects predefined hand gestures using **ESP32** and **flex sensors**, then plays corresponding voice outputs through a **DFPlayer Mini** audio module.

---

## 🧰 Tech Stack

| Layer | Technology |
|-------|-----------|
| MCU | ESP32 (Xtensa LX6, 240 MHz) |
| Sensors | 5× Analog Flex Sensors |
| Signal Acquisition | ESP32 ADC1 (12-bit) |
| Audio Playback | DFPlayer Mini over UART |
| Firmware | Embedded C (ESP-IDF) |
| Build System | CMake (ESP-IDF v5.x) |

---

## 🗂️ Repository Structure

```
gesture-vocalizer/
├── src/
│   ├── main.c                  # Entry point & FreeRTOS task
│   ├── gesture_recognition.c   # ADC acquisition + threshold classifier
│   └── dfplayer.c              # DFPlayer Mini UART driver
├── include/
│   ├── gesture_recognition.h
│   └── dfplayer.h
├── docs/
│   ├── wiring_diagram.md       # Pin connections
│   └── audio_tracks.md        # Track list for SD card
├── CMakeLists.txt
├── sdkconfig.defaults
└── README.md
```

---

## ⚡ Hardware Wiring

### Flex Sensors → ESP32 ADC

Each flex sensor forms a voltage divider with a **47kΩ** pull-down resistor.

| Finger | ESP32 GPIO | ADC Channel |
|--------|-----------|-------------|
| Thumb  | GPIO36    | ADC1_CH0   |
| Index  | GPIO39    | ADC1_CH3   |
| Middle | GPIO34    | ADC1_CH6   |
| Ring   | GPIO35    | ADC1_CH7   |
| Pinky  | GPIO32    | ADC1_CH4   |

> **Voltage divider:** 3.3V → Flex Sensor → ADC Pin → 47kΩ → GND

### DFPlayer Mini → ESP32 UART

| DFPlayer Pin | ESP32 GPIO |
|-------------|-----------|
| TX          | GPIO16 (RX1) |
| RX          | GPIO17 (TX1) |
| VCC         | 5V |
| GND         | GND |
| SPK_1/SPK_2 | 8Ω speaker |

---

## 🖐️ Gesture Dictionary

| Gesture | Hand Shape | Audio Track |
|---------|-----------|-------------|
| Hello | Open palm (all straight) | `001.mp3` |
| Thank You | Thumb up | `002.mp3` |
| Yes | Closed fist | `003.mp3` |
| No | Index finger point | `004.mp3` |
| Help | Peace / V sign | `005.mp3` |
| Water | W shape | `006.mp3` |

> Rename your MP3 files as `001.mp3`, `002.mp3`, … and copy them to the root of a FAT32-formatted microSD card inserted into the DFPlayer Mini.

---

## 🔧 Building & Flashing

### Prerequisites

- [ESP-IDF v5.x](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/)
- Python 3.8+

### Steps

```bash
# 1. Clone the repo
git clone https://github.com/<your-username>/gesture-vocalizer.git
cd gesture-vocalizer

# 2. Set up ESP-IDF environment
. $IDF_PATH/export.sh

# 3. Build
idf.py build

# 4. Flash (replace PORT with your serial port, e.g. /dev/ttyUSB0 or COM3)
idf.py -p PORT flash monitor
```

---

## 🎛️ Calibration

Flex sensors vary between units. After assembling the glove:

1. Open `include/gesture_recognition.h`
2. Adjust `FLEX_THRESHOLD_LOW` and `FLEX_THRESHOLD_HIGH` based on readings logged to serial monitor
3. Re-flash and verify gesture detection

---

## 📐 System Architecture

```
┌─────────────────────────────────────────────┐
│                   ESP32                      │
│                                              │
│  Flex Sensors ──► ADC1 ──► Threshold Logic  │
│                              │               │
│                       Gesture ID             │
│                              │               │
│                       DFPlayer Driver        │
│                              │               │
│                        UART1 (9600)          │
└──────────────────────────────┼──────────────┘
                               │
                        DFPlayer Mini
                               │
                          8Ω Speaker
```

---

## 🚀 Future Improvements

- [ ] ML-based gesture recognition (TensorFlow Lite for MCU)
- [ ] BLE companion app for custom vocabulary
- [ ] IMU integration for dynamic gesture support
- [ ] Rechargeable LiPo battery with low-power sleep modes

---

## 📄 License

MIT License – see [LICENSE](LICENSE) for details.
