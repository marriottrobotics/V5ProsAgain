#pragma once
#include "main.h"

namespace Movement{
  extern bool red;
  extern bool top;

  //void powerDrive(int leftF, int leftR, int rightF, int rightR);
  void powerMotor(int speed);
  void drive(int distance, int speed);
  void driveAsync(int distance, int speed);
  void turnUp(int distance, int speed);
  void slideUp(int distance, int speed);
  //This needs to be fixed.
  void alignUltrasonic(int rotateVelocity = 50, int threshold = 0, int delayTime = 5);
  void alignAlternate(float rotateVelocity = 25, int threshold = 3, int delayTime = 5);
  void alignTime(int rotateVelocity = 25, int time = 0, int delayTime = 5);
  void fireAuton();
  void towerPos(int newTarget, int speed);
  void towerSync(int newTarget, int speed);

  void moveAccel(int distance);
}
