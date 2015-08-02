// Import libraries (BLEPeripheral depends on SPI)
#include <SPI.h>
#include <BLEPeripheral.h>

#include <Servo.h> 

// define pins (varies per shield/board)
#define BLE_REQ   9
#define BLE_RDY   8
#define BLE_RST   7

#define MOTOR_1_CONTROL_PIN   6

// create peripheral instance, see pinouts above
BLEPeripheral            blePeripheral        = BLEPeripheral(BLE_REQ, BLE_RDY, BLE_RST);

// create service
BLEService               sliderService           = BLEService("51d0");

// create switch characteristic
BLECharCharacteristic    motor1Characteristic = BLECharCharacteristic("51d1", BLERead | BLEWrite);

Servo motor1;

void setup() {
  Serial.begin(9600);

  motor1.attach(MOTOR_1_CONTROL_PIN);

  // set advertised local name and service UUID
  blePeripheral.setLocalName("TheKickAssSlider");
  blePeripheral.setAdvertisedServiceUuid(sliderService.uuid());

  // add service and characteristic
  blePeripheral.addAttribute(sliderService);
  blePeripheral.addAttribute(motor1Characteristic);

  // begin initialization
  blePeripheral.begin();

  Serial.println(F("BLE The Kick Ass Slider Peripheral"));
}

void loop() {
  BLECentral central = blePeripheral.central();

  if (central) {
    // central connected to peripheral
    Serial.print(F("Connected to central: "));
    Serial.println(central.address());

    while (central.connected()) {
      // central still connected to peripheral
      if (motor1Characteristic.written()) {
        unsigned char inputValue = motor1Characteristic.value();

        int microSeconds = map(inputValue, -128, 127, 1000, 2000);

        motor1.writeMicroseconds(microSeconds);
      }
    }

    // central disconnected
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
