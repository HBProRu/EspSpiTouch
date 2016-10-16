//#include <Arduino.h>
#include <SPI.h>

//#include <Adafruit_ILI9341esp.h>
//#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_mfGFX.h>
#include <XPT2046.h>
#include "resources.h"

// Modify the following two lines to match your hardware
// Also, update calibration parameters below, as necessary

// For the esp shield, these are the default.
#define TFT_DC 2
#define TFT_CS 15
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

#define T_IRQ 3
#define T_CS 16
XPT2046 touch (T_CS, T_IRQ);

Adafruit_GFX_Button button, button2, buttonIcon_WiFi;

void setup() {
  delay(1000);

  Serial.begin(115200);

  tft.begin();
  tft.setRotation(3);
  touch.begin(tft.height(), tft.width());  // Must be done before setting rotation
  touch.setRotation(touch.ROT270);
  Serial.println("tftx ="); Serial.print(tft.width()); Serial.print(" tfty ="); Serial.println(tft.height());
  tft.fillScreen(ILI9341_BLACK);
  // Replace these for your screen module
  touch.setCalibration(209, 1759, 1775, 273);
  tft.setFont(OPENSANS_10);
  
  button.initButton(&tft, 107, 120, 70, 40, ILI9341_DARKCYAN, ILI9341_BLUE, ILI9341_GREENYELLOW, "Clear", 0);
  button.drawButton();
  button2.initButton(&tft, 213, 120, 70, 40, ILI9341_DARKCYAN, ILI9341_BLUE, ILI9341_GREENYELLOW, "Clear", 0);
  button2.drawButton();

  buttonIcon_WiFi.initButtonwithIcon(&tft, 121, 198, 72, 72, 48, 48, Icon_WiFi, ILI9341_BLUE, ILI9341_WHITE);
  buttonIcon_WiFi.drawButtonwithIcon();

  //tft.printAt("Hello",0,0,Left);

}

static uint16_t prev_x = 0xffff, prev_y = 0xffff;

void loop() {
    uint16_t x, y;
  if (touch.isTouching()) {
    touch.getPosition(x, y);
    Serial.print("x ="); Serial.print(x); Serial.print(" y ="); Serial.println(y);
    if (prev_x == 0xffff) {
      tft.drawPixel(x, y,ILI9341_BLUE);
    } else {
      tft.drawLine(prev_x, prev_y, x, y,ILI9341_BLUE);
    }
    prev_x = x;
    prev_y = y;
  } else {
    prev_x = prev_y = 0xffff;
  }


  button.press(button.contains(x, y)); // tell the button it is pressed
  button2.press(button2.contains(x, y));
  buttonIcon_WiFi.press(buttonIcon_WiFi.contains(x, y));
  
  // now we can ask the buttons if their state has changed
    if (button.justReleased()) {
        tft.fillScreen(ILI9341_BLACK);
        button.drawButton(); // draw normal
        button2.drawButton(); // draw normal
    }

    if (button.justPressed()) {
        button.drawButton(true); // draw invert!
    }

    if (buttonIcon_WiFi.justPressed()) {
        buttonIcon_WiFi.drawButtonwithIcon(true); // draw invert!
    }

    if (buttonIcon_WiFi.justReleased()) {
        buttonIcon_WiFi.drawButtonwithIcon(); // draw invert!
    }

    if (button2.justReleased()) {
        tft.fillScreen(ILI9341_GREEN);
        button.drawButton(); // draw normal
        button2.drawButton(); // draw normal
    }

    if (button2.justPressed()) {
        button2.drawButton(true); // draw invert!
    }

  delay(20);
}
