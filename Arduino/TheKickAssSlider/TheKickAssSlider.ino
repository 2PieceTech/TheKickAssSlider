// Import libraries (BLEPeripheral depends on SPI)
#include <SPI.h>
#include <BLEPeripheral.h>

#include <Servo.h> 

// define pins (varies per shield/board)
#define BLE_REQ   9
#define BLE_RDY   8
#define BLE_RST   7

#define MOTOR_1_CONTROL_PIN   6
#define MOTOR_2_CONTROL_PIN   5
#define MOTOR_3_CONTROL_PIN   4
#define MOTOR_4_CONTROL_PIN   3

// create peripheral instance, see pinouts above
BLEPeripheral            blePeripheral        = BLEPeripheral(BLE_REQ, BLE_RDY, BLE_RST);

// create service
BLEService               sliderService           = BLEService("51d0");

// create motor characteristics
BLECharCharacteristic    motor1Characteristic = BLECharCharacteristic("51d1", BLERead | BLEWrite);
BLECharCharacteristic    motor2Characteristic = BLECharCharacteristic("51d2", BLERead | BLEWrite);
BLECharCharacteristic    motor3Characteristic = BLECharCharacteristic("51d3", BLERead | BLEWrite);
BLECharCharacteristic    motor4Characteristic = BLECharCharacteristic("51d4", BLERead | BLEWrite);

Servo motor1;
Servo motor2;
Servo motor3;
Servo motor4;

void setup() {
  Serial.begin(9600);

  motor1.attach(MOTOR_1_CONTROL_PIN);
  motor2.attach(MOTOR_2_CONTROL_PIN);
  motor3.attach(MOTOR_3_CONTROL_PIN);
  motor4.attach(MOTOR_4_CONTROL_PIN);

  // set advertised local name and service UUID
  blePeripheral.setLocalName("TheKickAssSlider");
  blePeripheral.setAdvertisedServiceUuid(sliderService.uuid());

  // add service and characteristic
  blePeripheral.addAttribute(sliderService);
  blePeripheral.addAttribute(motor1Characteristic);
  blePeripheral.addAttribute(motor2Characteristic);
  blePeripheral.addAttribute(motor3Characteristic);
  blePeripheral.addAttribute(motor4Characteristic);

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
        updateMotorSpeed(motor1, motor1Characteristic.value());
      }

      if (motor2Characteristic.written()) {
        updateMotorSpeed(motor2, motor2Characteristic.value());
      }

      if (motor3Characteristic.written()) {
        updateMotorSpeed(motor3, motor3Characteristic.value());
      }

      if (motor4Characteristic.written()) {
        updateMotorSpeed(motor4, motor4Characteristic.value());
      }
    }

    // central disconnected
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}

void updateMotorSpeed(Servo& motor, unsigned char speed) {
  int microSeconds = map(speed, -128, 127, 1000, 2000);

  motor.writeMicroseconds(microSeconds);
}

