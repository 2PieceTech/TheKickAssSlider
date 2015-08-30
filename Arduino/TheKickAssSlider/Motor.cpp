#include <Arduino.h>

#include "Motor.h"

Motor::Motor() :
  Servo()
{
}

void Motor::setSpeed(int speed) {
  int microSeconds = map(speed, -128, 127, 1000, 2000);

  this->writeMicroseconds(microSeconds);
}

