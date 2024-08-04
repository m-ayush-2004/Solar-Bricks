/*
 * This ESP8266 NodeMCU code was developed by newbiely.com
 *
 * This ESP8266 NodeMCU code is made available for public use without any restriction
 *
 * For comprehensive instructions and wiring diagrams, please visit:
 * https://newbiely.com/tutorials/esp8266/esp8266-ir-remote-control
 */

#include <DIYables_IRcontroller.h> // DIYables_IRcontroller library
#define IR_RECEIVER_PIN A0 // The ESP8266 pin D5 connected to IR controller

DIYables_IRcontroller_17 irController(IR_RECEIVER_PIN, 200); // debounce time is 200ms

void setup() {
  Serial.begin(9600);
  irController.begin();
}

void loop() {
  Key17 key = irController.getKey();
  if (key != Key17::NONE) {
    switch (key) {
      case Key17::KEY_1:
        Serial.println("1");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_2:
        Serial.println("2");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_3:
        Serial.println("3");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_4:
        Serial.println("4");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_5:
        Serial.println("5");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_6:
        Serial.println("6");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_7:
        Serial.println("7");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_8:
        Serial.println("8");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_9:
        Serial.println("9");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_STAR:
        Serial.println("*");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_0:
        Serial.println("0");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_SHARP:
        Serial.println("#");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_UP:
        Serial.println("UP");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_DOWN:
        Serial.println("DOWN");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_LEFT:
        Serial.println("LEFT");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_RIGHT:
        Serial.println("RIGHT");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_OK :
        Serial.println("OK");
        // TODO: YOUR CONTROL
        break;

      default:
        Serial.println("WARNING: undefined key:");
        break;
    }
  }
}
