// #include <BleGamepad.h>

// BleGamepad gamepad("MyESP32Gamepad", "ESP32", 100);

// void setup() {
//   BleGamepadConfiguration config;
//   config.setWhichAxes(true, true, false, false, false, false, false, false); // X,Y only
//   config.setButtonCount(3);
//   gamepad.begin(&config);
// }

// void loop() {
//   if (gamepad.isConnected()) {

//     // Buttons
//     if (greenPressed) gamepad.press(1); else gamepad.release(1);
//     if (redPressed)   gamepad.press(2); else gamepad.release(2);
//     if (bluePressed)  gamepad.press(10); else gamepad.release(10);

//     // Joystick from accelerometer
//     int x = (int) (ax * 127);   // scale float → -127..127
//     int y = (int) (ay * 127);

//     gamepad.setAxes(x, y, 0, 0, 0, 0);
//     gamepad.sendReport();
//   }

//   delay(10);
// }
