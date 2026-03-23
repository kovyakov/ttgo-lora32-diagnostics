/*
 * LoRa Radio Ping Test — TTGO LoRa32 V1.3
 *
 * Sends a raw LoRa packet every 5 seconds on 916.8 MHz (AU915 FSB2, ch8).
 * No LoRaWAN — just bare SX1276 TX to verify the radio hardware works
 * and the gateway can hear it.
 *
 * If the gateway sees these packets in Live Data as "Receive uplink message",
 * the radio is fine and the problem is in the OTAA join (credentials/config).
 *
 * If the gateway sees NOTHING, the radio or antenna has a problem.
 *
 * Board:  TTGO LoRa32 V1.3
 * Libs:   LoRa by Sandeepmistry, Adafruit SSD1306, Adafruit GFX
 * Pins:   SCK=5, MISO=19, MOSI=27, NSS=18, RST=14, DIO0=26
 */

#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ── LoRa32 V1.3 SX1276 pins ────────────────────────────────────
#define LORA_SCK   5
#define LORA_MISO  19
#define LORA_MOSI  27
#define LORA_NSS   18
#define LORA_RST   14
#define LORA_DIO0  26

// ── OLED pins ───────────────────────────────────────────────────
#define OLED_SDA    4
#define OLED_SCL   15
#define OLED_RST   16

// ── LED ─────────────────────────────────────────────────────────
#define LED_PIN    25

// ── Radio config (AU915 FSB2) ───────────────────────────────────
#define FREQUENCY     916800000  // 916.8 MHz — AU915 channel 8 (FSB2 first ch)
#define BANDWIDTH     125000     // 125 kHz
#define SPREAD_FACTOR 10         // SF10 — same as join attempts
#define TX_POWER      20         // dBm (max for SX1276)
#define SYNC_WORD     0x34       // LoRaWAN public sync word

// ── OLED (SSD1306 128x64) ──────────────────────────────────────
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT  64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

int packetCount = 0;

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("\n=== LoRa Ping Test ===");
  Serial.println("Board: TTGO LoRa32 V1.3");
  Serial.printf("Freq:  %.1f MHz  SF%d  BW%dk\n",
                FREQUENCY / 1e6, SPREAD_FACTOR, BANDWIDTH / 1000);

  // ── LED ───────────────────────────────────────────────────────
  pinMode(LED_PIN, OUTPUT);

  // ── OLED ──────────────────────────────────────────────────────
  Wire.begin(OLED_SDA, OLED_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED init failed");
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("LoRa Ping Test");
  display.println("Initializing...");
  display.display();

  // ── SPI + LoRa ────────────────────────────────────────────────
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_NSS);
  LoRa.setPins(LORA_NSS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(FREQUENCY)) {
    Serial.println("ERROR: LoRa init failed!");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("LoRa INIT FAILED");
    display.println("Check SX1276");
    display.display();
    while (1) {
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
      delay(200);
    }
  }

  // Configure radio to match LoRaWAN-like settings
  LoRa.setSpreadingFactor(SPREAD_FACTOR);
  LoRa.setSignalBandwidth(BANDWIDTH);
  LoRa.setCodingRate4(5);
  LoRa.setSyncWord(SYNC_WORD);
  LoRa.setTxPower(TX_POWER);
  LoRa.enableCrc();

  Serial.println("LoRa radio OK. Sending pings every 5 seconds...\n");

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("LoRa Ping Test");
  display.println("Radio OK!");
  display.println("Sending pings...");
  display.display();
  delay(1000);
}

void loop() {
  packetCount++;

  // Build a simple test payload
  char msg[64];
  snprintf(msg, sizeof(msg), "PING #%d from OrchardMonitor", packetCount);

  // Transmit
  digitalWrite(LED_PIN, HIGH);
  Serial.printf("[TX] Sending packet #%d on %.1f MHz ... ", packetCount, FREQUENCY / 1e6);

  LoRa.beginPacket();
  LoRa.print(msg);
  LoRa.endPacket();

  Serial.println("done");
  digitalWrite(LED_PIN, LOW);

  // Update OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("LoRa Ping Test");
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

  display.setCursor(0, 14);
  display.printf("Freq: %.1f MHz", FREQUENCY / 1e6);

  display.setCursor(0, 26);
  display.printf("SF%d  BW%dk  %ddBm", SPREAD_FACTOR, BANDWIDTH / 1000, TX_POWER);

  display.setCursor(0, 40);
  display.printf("Packets sent: %d", packetCount);

  display.setCursor(0, 54);
  display.print("Check gateway data!");
  display.display();

  delay(5000);  // ping every 5 seconds
}
