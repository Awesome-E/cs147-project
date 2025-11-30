#include <Arduino.h>
#include <SparkFunLSM6DSO.h>
#include <Wire.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BleGamepad.h>
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define RED_BUTTON_PIN 2
#define BLUE_BUTTON_PIN 15
#define GREEN_BUTTON_PIN 13

#define DEAD_ZONE_THRESHOLD 0.12
#define JOYSTICK_SCALE_FACTOR 3

LSM6DSO acc;
BLECharacteristic *pCharacteristic;

void initButtonPins() {
    pinMode(RED_BUTTON_PIN, INPUT);
    pinMode(BLUE_BUTTON_PIN, INPUT);
    pinMode(GREEN_BUTTON_PIN, INPUT);
}

void initAccelerometer() {
    delay(500);
    Wire.begin();
    delay(10);

    if (acc.begin()) {
        Serial.println("Ready.");
    } else {
        Serial.println("Could not connect to IMU.");
        Serial.println("Freezing");
    }

    if (acc.initialize(BASIC_SETTINGS)) {
        Serial.println("Loaded Settings.");
    }
}

// void setupBluetooth() {
//     Serial.println("Starting BLE work!");
//     BLEDevice::init("CS147");
//     BLEServer *pServer = BLEDevice::createServer();
//     BLEService *pService = pServer->createService(SERVICE_UUID);
//     pCharacteristic = pService->createCharacteristic(
//         CHARACTERISTIC_UUID,
//         BLECharacteristic::PROPERTY_READ |
//             BLECharacteristic::PROPERTY_WRITE);
//     pCharacteristic->setValue("Server Example – CS147"); // What gets transfered
//     pService->start();
//     BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
//     pAdvertising->addServiceUUID(SERVICE_UUID);
//     pAdvertising->setScanResponse(true);
//     pAdvertising->setMinPreferred(0x0);
//     pAdvertising->setMinPreferred(0x12);
//     BLEDevice::startAdvertising();
//     Serial.println("Characteristic defined! Now you can read it in your phone!");
// }

BleGamepad gamepad;
BleGamepadConfiguration config;

void setupGamepad() {
    gamepad = BleGamepad("MyESP32Gamepad", "ESP32", 100);
    config = BleGamepadConfiguration();
    config.setWhichAxes(true, true, false, false, false, false, false, false); // X,Y only
    config.setButtonCount(15); // issues
    gamepad.begin(&config);
}

void setup() {
    Serial.begin(9600);
    initButtonPins();
    initAccelerometer();
    // setupBluetooth();
    setupGamepad();
}
void transmitGamepadData(int redPressed, int greenPressed, int bluePressed, float ax, float ay) {
    // transmit button presses
    if (greenPressed) gamepad.press(1);  else gamepad.release(1);
    if (redPressed)   gamepad.press(2);  else gamepad.release(2);
    if (bluePressed)  gamepad.press(10); else gamepad.release(10);

    // transmit axes - map range [-1.000, 1.000] to [0, 32767]
    
    if (abs(ax) < DEAD_ZONE_THRESHOLD) ax = 0; else {
        // scale inputs to match physical joystick better
        ax = (ax + (ax > 0 ? -DEAD_ZONE_THRESHOLD : DEAD_ZONE_THRESHOLD)) * JOYSTICK_SCALE_FACTOR;
    }
    if (abs(ay) < DEAD_ZONE_THRESHOLD) ay = 0; else {
      ay = (ay + (ay > 0 ? -DEAD_ZONE_THRESHOLD : DEAD_ZONE_THRESHOLD)) * JOYSTICK_SCALE_FACTOR;
    }

    int x = (int) constrain(ax * 16384 + 16384, 0, 32767);
    int y = (int) constrain(ay * 16384 + 16384, 0, 32767);

    Serial.printf("ax=%d,ay=%d,az=%d   \r", x, y, 0);
    gamepad.setAxes(x, y, 0, 0, 0, 0);
}

void loop() {
    delay(5);

    // get accelerometer readings
    float ax = acc.readFloatAccelX();
    float ay = acc.readFloatAccelY();
    float az = acc.readFloatAccelZ();

    // print when button gets pressed or not pressed
    int redPressed = digitalRead(RED_BUTTON_PIN);
    int bluePressed = digitalRead(BLUE_BUTTON_PIN);
    int greenPressed = digitalRead(GREEN_BUTTON_PIN);

    // Serial.printf("ax=%.3f,\t ay=%.3f,\t az=%.3f\n", ax, ay, az);
    // Serial.printf("red=%d, blue=%d, green=%d\n\n", redPressed, bluePressed, greenPressed);

    // char data[64];
    // sprintf(data, "ax=%.2f,ay=%.2f,az=%.2f,r=%d,g=%d,b=%d", 
    //         acc.readFloatAccelX(), acc.readFloatAccelY(), acc.readFloatAccelZ(),
    //         digitalRead(RED_BUTTON_PIN), digitalRead(GREEN_BUTTON_PIN), digitalRead(BLUE_BUTTON_PIN));
    // pCharacteristic->setValue(data);

    if (gamepad.isConnected()) {
        transmitGamepadData(redPressed, greenPressed, bluePressed, ax, ay);
    }
}
