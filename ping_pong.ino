#include <FastLED.h>

#define LED_PIN     6
#define NUM_LEDS    20
#define BUTTON_PIN  2

CRGB leds[NUM_LEDS];

int delayTime = 50;
int tailLength = 1;
bool directionRight = true;
bool lastButtonState = HIGH;

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  bool buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == LOW && lastButtonState == HIGH) {
    directionRight = !directionRight;
    delay(20); // антидребезг
  }
  lastButtonState = buttonState;

  if (directionRight) {
    for (int i = 0; i < NUM_LEDS + tailLength; i++) {
      drawTail(i, directionRight);
      if (checkButton()) return;
    }
    directionRight = false;
  } else {
    for (int i = NUM_LEDS - 1; i >= -tailLength; i--) {
      drawTail(i, directionRight);
      if (checkButton()) return;
    }
    directionRight = true;
  }
}

void drawTail(int head, bool dir) {
  // сначала задаём базовые цвета
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i < 5) leds[i] = CRGB::Red;                // левая сторона
    else if (i >= NUM_LEDS - 5) leds[i] = CRGB::Green; // правая сторона
    else leds[i] = CRGB::Black;                    // середина — чёрная
  }

  // рисуем синий бегущий огонёк с хвостом
  for (int t = 0; t <= tailLength; t++) {
    int pos = dir ? head - t : head + t;
    if (pos >= 0 && pos < NUM_LEDS) {
      uint8_t brightness = 255 - (200 / (tailLength + 1)) * t;
      leds[pos] = CHSV(160, 255, brightness); // синий
    }
  }

  FastLED.show();
  delay(delayTime);
}

bool checkButton() {
  bool state = digitalRead(BUTTON_PIN);
  if (state == LOW && lastButtonState == HIGH) {
    directionRight = !directionRight;
    lastButtonState = state;
    delay(20);
    return true;
  }
  lastButtonState = state;
  return false;
}
