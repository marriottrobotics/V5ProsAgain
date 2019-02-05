#pragma once
#include "main.h"

namespace Movement{
  extern bool red;
  extern bool top;

  void powerDrive(int leftF, int leftR, int rightF, int rightR);
  void powerMotor(int speed);
  void drive(int distance, int speed);
  void driveAsync(int distance, int speed);
  void turnUp(int distance, int speed);
  void slideUp(int distance, int speed);
  void alignUltrasonic();
  void fireAuton();
  void towerPos(int newTarget, int speed);
  void towerSync(int newTarget, int speed);
}
