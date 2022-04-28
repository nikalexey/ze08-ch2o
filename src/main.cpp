#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>
#include "ze08_ch2o.h"
#include <SoftwareSerial.h>
#include "codic.h"

SoftwareSerial ch2oSerial(D3, D4); // RX, TX
// Instantiate a sensor connected to the previous port
Ze08CH2O ch2o{&ch2oSerial};

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3c  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define DIM_DISPLAY false // you can specify true for little bit less bright display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
uint32_t logoTimeout = 5 * 1000;
uint32_t warmingUpTimeout = 30 * 1000;


void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
  ch2oSerial.begin(9600);
  Serial.println("Quadcode");
  display.dim(DIM_DISPLAY);
  display.clearDisplay();
  display.drawBitmap(0, 0, codic_bitmap, codic_width, codic_height, 1);
  display.display();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(SCREEN_WIDTH/2+6, 15);
  display.println(F("Quad"));
  display.setCursor(SCREEN_WIDTH/2+6, 35);
  display.println(F("code"));
  display.display();
  delay(logoTimeout);

  display.clearDisplay();
  display.setCursor(5, 8);
  display.println(F("WARMING UP"));
  auto timeout = millis();
  display.drawRect(0, 54, 128, 10, 1);
  while (timeout + warmingUpTimeout > millis()) {
    auto x = map(millis() - timeout, 0, warmingUpTimeout, 1, 126);
    display.fillRect(1, 55, x, 9, 1);
    display.display();
  }
}

void loop() {
  Ze08CH2O::concentration_t reading;
  if (ch2o.read(reading)) {
    display.clearDisplay();
    display.drawRect(10, 10, 108, 44, 1);
    display.drawLine(15, 10, 80, 10, 0);
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(18, 8);
    display.println("CH2O level");

    display.setTextSize(3);
    display.setCursor(25, 24);
    display.println(reading);
    display.display();
  }
  
}