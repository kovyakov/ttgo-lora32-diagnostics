# ttgo-lora32-diagnostics

Hardware diagnostic toolkit for TTGO LoRa32 boards. Two standalone Arduino sketches for verifying that the OLED display and LoRa radio are working correctly.

## Board

TTGO LoRa32 V1.3 (ESP32 + SX1276 + SSD1306 128×64 OLED)

## Sketches

### `oled_test.ino` — OLED display test

Minimal "hello world" that resets and initialises the on-board SSD1306 OLED, then prints **"Privet, mir!"** on screen. Use this first to confirm the display works before running more complex sketches.

| Pin   | GPIO |
|-------|------|
| SDA   | 21   |
| SCL   | 22   |
| RST   | 16   |

### `lora_test.ino` — LoRa radio ping test

Sends a raw LoRa packet every 5 seconds on **916.8 MHz** (AU915 FSB2, channel 8). This is bare SX1276 TX — no LoRaWAN stack — so you can verify the radio hardware independently of OTAA join issues.

**How to interpret the results:**

- **Gateway shows "Receive uplink message"** → radio and antenna are fine; the problem is in OTAA credentials/config.
- **Gateway shows nothing** → radio or antenna has a hardware problem.

Radio settings:

| Parameter        | Value          |
|------------------|----------------|
| Frequency        | 916.8 MHz      |
| Bandwidth        | 125 kHz        |
| Spreading Factor | SF10           |
| TX Power         | 20 dBm         |
| Sync Word        | 0x34 (LoRaWAN) |
| CRC              | Enabled        |

Pin mapping:

| Pin   | GPIO |
|-------|------|
| SCK   | 5    |
| MISO  | 19   |
| MOSI  | 27   |
| NSS   | 18   |
| RST   | 14   |
| DIO0  | 26   |
| LED   | 25   |

The OLED (SDA=4, SCL=15, RST=16) is also used in this sketch to display live packet count and radio parameters.

## Dependencies

Install these via the Arduino Library Manager:

- **LoRa** by Sandeep Mistry
- **Adafruit SSD1306**
- **Adafruit GFX Library**

## Quick start

1. Install the libraries listed above.
2. In the Arduino IDE select board **TTGO LoRa32-OLED**.
3. Upload `oled_test.ino` first — confirm text appears on the display.
4. Upload `lora_test.ino` — open your gateway's live data view and check for incoming packets.