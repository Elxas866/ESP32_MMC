#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Encoder.h>
#include <BleKeyboard.h>

// Config of OLED display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Config of rotary rncoder
#define ENCODER_PIN1 27
#define ENCODER_PIN2 14
#define ENCODER_BUTTON 25
ESP32Encoder encoder;
// last count
int64_t encoderLastCount;

// config of BLE keyboard
BleKeyboard bleKeyboard("ESP32 BLE MMC", "Elias Kelta", 100);

// put function declarations here:

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
	// Enable the weak pull down resistors

	//ESP32Encoder::useInternalWeakPullResistors = puType::down;
	// Enable the weak pull up resistors
	ESP32Encoder::useInternalWeakPullResistors = puType::up;
  encoder.attachHalfQuad(ENCODER_PIN1, ENCODER_PIN2);

  // set starting count value after attaching
	encoder.setCount(50);

  Serial.println("Encoder Start = " + String((int32_t)encoder.getCount()));
  encoderLastCount = encoder.getCount();

  Serial.println("Starting BLE work!");
  bleKeyboard.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Encoder Count = " + String((int32_t)encoder.getCount()));

  if(bleKeyboard.isConnected()) { 
      int64_t newCount = encoder.getCount();
      if (newCount != encoderLastCount) {
        if (newCount > encoderLastCount) {
          bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
          Serial.println("Volume up");
        } else if (newCount < encoderLastCount) {
          bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
          Serial.println("Volume down");
        }
        encoderLastCount = encoder.getCount();
    }
  }
}