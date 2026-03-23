/*
 * OLED Hello World — TTGO LoRa32 (SDA=21, SCL=22)
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_SDA  21
#define OLED_SCL  22
#define OLED_RST  16

Adafruit_SSD1306 display(128, 64, &Wire, OLED_RST);

void setup() {
  Serial.begin(115200);

  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  Wire.begin(OLED_SDA, OLED_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false)) {
    Serial.println("OLED not found!");
    for (;;);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(8, 20);
  display.println("Privet, mir!");
  display.display();

  Serial.println("Privet, mir! on OLED!");
}

void loop() {
}
