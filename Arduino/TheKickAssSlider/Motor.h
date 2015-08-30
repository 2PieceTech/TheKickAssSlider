#include <Servo.h>

#ifndef _MOTOR_H
#define _MOTOR_H

class Motor : public Servo
{

public:
  Motor();
  void setSpeed(int speed);  
};

#endif


