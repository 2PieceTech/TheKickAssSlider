// Import libraries (BLEPeripheral depends on SPI)
#include <SPI.h>
#include <BLEPeripheral.h>

#include <Servo.h>

#include "Motor.h"

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
BLEUnsignedLongCharacteristic motorCharacteristic = BLEUnsignedLongCharacteristic("51d1", BLERead | BLEWrite);

#define NUM_MOTORS 4
Motor motors[NUM_MOTORS];

void setup() {
  Serial.begin(9600);

  motors[0].attach(MOTOR_1_CONTROL_PIN);
  motors[1].attach(MOTOR_2_CONTROL_PIN);
  motors[2].attach(MOTOR_3_CONTROL_PIN);
  motors[3].attach(MOTOR_4_CONTROL_PIN);

  // set advertised local name and service UUID
  blePeripheral.setLocalName("TheKickAssSlider");
  blePeripheral.setAdvertisedServiceUuid(sliderService.uuid());

  // add service and characteristic
  blePeripheral.addAttribute(sliderService);
  blePeripheral.addAttribute(motorCharacteristic);

  // assign event handlers for connected, disconnected to peripheral
  blePeripheral.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  blePeripheral.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  // assign event handlers for characteristic
  motorCharacteristic.setEventHandler(BLEWritten, motorCharacteristicWritten);

  // begin initialization
  blePeripheral.begin();

  Serial.println(F("BLE The Kick Ass Slider Peripheral"));
}

void loop() {
  // poll peripheral
  blePeripheral.poll();
}

void blePeripheralConnectHandler(BLECentral& central) {
  // central connected event handler
  Serial.print(F("Connected event, central: "));
  Serial.println(central.address());

  setMotorSpeeds(0);
}

void blePeripheralDisconnectHandler(BLECentral& central) {
  // central disconnected event handler
  Serial.print(F("Disconnected event, central: "));
  Serial.println(central.address());

  setMotorSpeeds(0);
}

void motorCharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic) {
  // central wrote new value to characteristic
  Serial.print(F("Characteristic event, writen: "));

  setMotorSpeeds(motorCharacteristic.value());
}


void setMotorSpeeds(unsigned long motorSpeeds) {
  char speeds[NUM_MOTORS];

   memcpy(&speeds, &motorSpeeds, sizeof(speeds));
  
  for (int i = 0; i < NUM_MOTORS; i++) {
    motors[i].setSpeed(speeds[i]);
  }
}

