#include "SPI.h"
#include "Adafruit_GFX.h"
#include <SPI.h>
#include <Wire.h>
#include "Adafruit_ILI9341.h"
#include <Adafruit_STMPE610.h>

#define TFT_DC 9
#define TFT_CS 10
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
#define STMPE_CS 8
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

#define TS_MINX 150
#define TS_MINY 130
#define TS_MAXX 3800
#define TS_MAXY 4000

void setup() {
  Serial.begin(9600);
  Serial.println("ILI9341 Test!"); 
  tft.begin();
  ts.begin();
  //tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
  yield();
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("Hello, world!");
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_GREEN);
  tft.println("Julien is in da houuuuse!!!");
}

void loop() {
  if(!ts.bufferEmpty())
  {
    TS_Point p=ts.getPoint();
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
    tft.fillCircle(p.x, p.y, 10, ILI9341_YELLOW);
    //Serial.print("X = "); Serial.print(p.x);
    //Serial.print("\tY = "); Serial.print(p.y);
    //Serial.println("\tPressure = "); Serial.println(p.z);  
  }
  //delay(100);
}
