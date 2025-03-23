#include <Arduino.h>
#include <FastLED.h>
#include "hsl.h"

#define LED_BUILTIN 23
#define LED_PIN 24
// one singlular LED btw
#define BUTTON_PIN 25
#define DEBOUNCE_DELAY 10

CRGB leds[1];

void setup()
{
  Serial.begin(9600);

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, 1);
  FastLED.setBrightness(255);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

int hue = 0;
int velocity = 2;

int brightness = 255;

enum LedState
{
  Off,
  GreenOn,
  RGBOn,
  GreenAndRGBOn,
};

LedState ledState = GreenAndRGBOn;

bool lastButtonState = false;
int debounceCounter = 0;

void getButtonState()
{
  bool buttonState = !digitalRead(BUTTON_PIN);
  if (buttonState != lastButtonState && debounceCounter > 0)
  {
    debounceCounter -= 1;
  }
  else
  {
    debounceCounter = DEBOUNCE_DELAY;
  }

  Serial.print("DebounceCounter: ");
  Serial.println(debounceCounter);

  Serial.print("ButtonState: ");
  Serial.println(buttonState);

  Serial.print("LastButtonState: ");
  Serial.println(lastButtonState);

  if (debounceCounter <= 0)
  {
    lastButtonState = buttonState;

    if (buttonState)
    {
      ledState = (LedState)((ledState + 1) % 4);
      Serial.print("LedState: ");
      Serial.println(ledState);
    }
  }
}

void loop()
{
  getButtonState();

  hue = (hue + velocity) % 360;

  float h = hue / 360.0;

  TobyRGB rgb = hsl2rgb(h, 1, 0.5);

  Serial.print("Hue: ");
  Serial.println(hue);

  leds[0] = CRGB(rgb.r, rgb.g, rgb.b);
  if (ledState == Off || ledState == GreenOn)
  {
    leds[0] = CRGB(0, 0, 0);
  }

  FastLED.show();

  brightness = brightness - 1;
  if (brightness < 0)
  {
    brightness = 255;
  }

  if (ledState == Off || ledState == RGBOn)
  {
    analogWrite(LED_BUILTIN, 0);
  }
  else
  {
    analogWrite(LED_BUILTIN, brightness);
  }

  delay(10);
}
