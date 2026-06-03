# Wiring Diagram

## Flex Sensor Voltage Divider Circuit

Each flex sensor is connected in a voltage divider configuration:

```
3.3V
  │
  ├── [Flex Sensor] ──┬── ADC Pin (ESP32)
                      │
                    [47kΩ]
                      │
                     GND
```

When the finger is **straight**, resistance is low → higher ADC voltage.  
When the finger is **bent**, resistance increases → lower ADC voltage.

## Full Wiring Table

| Component         | Pin         | ESP32 Pin | Notes              |
|-------------------|-------------|-----------|-------------------|
| Flex Sensor (Thumb)   | Output  | GPIO36    | + 47kΩ to GND     |
| Flex Sensor (Index)   | Output  | GPIO39    | + 47kΩ to GND     |
| Flex Sensor (Middle)  | Output  | GPIO34    | + 47kΩ to GND     |
| Flex Sensor (Ring)    | Output  | GPIO35    | + 47kΩ to GND     |
| Flex Sensor (Pinky)   | Output  | GPIO32    | + 47kΩ to GND     |
| DFPlayer Mini         | TX      | GPIO16    | 1kΩ series recommended |
| DFPlayer Mini         | RX      | GPIO17    | 1kΩ series recommended |
| DFPlayer Mini         | VCC     | 5V        | Use 5V rail        |
| DFPlayer Mini         | GND     | GND       |                   |
| Speaker (8Ω)          | SPK_1/2 | DFPlayer  | Max 3W            |

## Power Supply

- ESP32 powered via USB or 3.7V LiPo with TP4056 charger module
- DFPlayer Mini requires 5V — use a small boost converter if on battery
