#include <Arduino.h>
#include <SparkFunLSM6DSO.h>
#include <Wire.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define RED_BUTTON_PIN 2
#define BLUE_BUTTON_PIN 15
#define GREEN_BUTTON_PIN 13

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

void setupBluetooth() {
    Serial.println("Starting BLE work!");
    BLEDevice::init("CS147");
    BLEServer *pServer = BLEDevice::createServer();
    BLEService *pService = pServer->createService(SERVICE_UUID);
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE);
    pCharacteristic->setValue("Server Example – CS147"); // What gets transfered
    pService->start();
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x0);
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
    Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void setup() {
    Serial.begin(9600);
    initButtonPins();
    initAccelerometer();
    setupBluetooth();
}

void loop() {
    delay(80);

    // print accelerometer readings
    float gx = acc.readFloatAccelX();
    float gy = acc.readFloatAccelY();
    float gz = acc.readFloatAccelZ();

    // print when button gets pressed or not pressed
    int redPressed = digitalRead(RED_BUTTON_PIN);
    int bluePressed = digitalRead(BLUE_BUTTON_PIN);
    int greenPressed = digitalRead(GREEN_BUTTON_PIN);

    Serial.printf("gx=%.3f,\t gy=%.3f,\t gz=%.3f\n", gx, gy, gz);
    Serial.printf("red=%d, blue=%d, green=%d\n\n", redPressed, bluePressed, greenPressed);

    char data[64];
    sprintf(data, "gx=%.2f,gy=%.2f,gz=%.2f,r=%d,g=%d,b=%d", 
            acc.readFloatAccelX(), acc.readFloatAccelY(), acc.readFloatAccelZ(),
            digitalRead(RED_BUTTON_PIN), digitalRead(GREEN_BUTTON_PIN), digitalRead(BLUE_BUTTON_PIN));
    pCharacteristic->setValue(data);
}
