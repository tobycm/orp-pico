#include <Arduino.h>
#include <FastLED.h>
#include "hsl.h"

#define LED_BUILTIN 23
#define LED_PIN 26

#define NUM_LEDS 144

#define BUTTON_PIN 25
#define DEBOUNCE_DELAY 10

CRGB leds[NUM_LEDS];

int brightness = 200;

void setup()
{
  Serial.begin(9600);

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(brightness);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

float hue = 0;
float velocity = 360 / NUM_LEDS;

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

void rainbowLeds()
{
  hue += velocity;

  if (hue >= 360)
  {
    hue = 0;
  }

  Serial.print("Hue: ");
  Serial.println(hue);
}

int currentLed = 0;

void cycleLeds()
{
  float h = hue / 360.0;

  TobyRGB rgb = hsl2rgb(h, 1, 0.5);

  leds[currentLed - 7] = CRGB::Black;

  if (currentLed - 7 < 0)
  {
    leds[NUM_LEDS + currentLed - 7] = CRGB::Black;
  }

  leds[currentLed] = CRGB(rgb.r, rgb.g, rgb.b);

  currentLed = (currentLed + 1) % NUM_LEDS;

  Serial.print("CurrentLed: ");
  Serial.println(currentLed);

  FastLED.show();
}

void loop()
{
  // getButtonState();

  rainbowLeds();

  cycleLeds();

  delay(20);
}
