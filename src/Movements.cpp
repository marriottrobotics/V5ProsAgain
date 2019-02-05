#include "Movements.h"
#include <iostream>

bool Movement::red = false;
bool Movement::top = false;

void Movement::powerMotor(int speed){
  leftDriveF.move_velocity(speed);
  leftDriveR.move_velocity(speed);
  rightDriveF.move_velocity(speed);
  rightDriveR.move_velocity(speed);
}

void Movement::drive(int distance, int speed){
  leftDriveF.move_relative(distance, speed);
  leftDriveR.move_relative(distance, speed);
  rightDriveF.move_relative(distance, speed);
  rightDriveR.move_relative(distance, speed);
  while(ABS(rightDriveR.get_position()-rightDriveR.get_target_position()) >= 10){
    //Do nothing
  }
}

void Movement::driveAsync(int distance, int speed){
  leftDriveF.move_relative(distance, speed);
  leftDriveR.move_relative(distance, speed);
  rightDriveF.move_relative(distance, speed);
  rightDriveR.move_relative(distance, speed);
}

void Movement::turnUp(int distance, int speed){
    if(!red){
      leftDriveF.move_relative(distance, speed);
      leftDriveR.move_relative(distance, speed);
      rightDriveF.move_relative(-distance, speed);
      rightDriveR.move_relative(-distance, speed);
    }else{
      leftDriveF.move_relative(-distance, speed);
      leftDriveR.move_relative(-distance, speed);
      rightDriveF.move_relative(distance, speed);
      rightDriveR.move_relative(distance, speed);
    }
    while(ABS(rightDriveR.get_position()-rightDriveR.get_target_position()) >= 10){
      //Do nothing
    }
}

void Movement::slideUp(int distance, int speed){
    if(!red){
      leftDriveF.move_relative(distance, speed);
      leftDriveR.move_relative(-distance, speed);
      rightDriveF.move_relative(-distance, speed);
      rightDriveR.move_relative(distance, speed);
    }else{
      leftDriveF.move_relative(-distance, speed);
      leftDriveR.move_relative(distance, speed);
      rightDriveF.move_relative(distance, speed);
      rightDriveR.move_relative(-distance, speed);
    }
    while(ABS(rightDriveR.get_position()-rightDriveR.get_target_position()) >= 10){
      //Do nothing
    }
}

void Movement::alignUltrasonic(int rotateVelocity, int threshold, int delayTime){
  //printf("Ultra align started \n");
  //printf("Left %d, Right %d \n", ultraLeft.get_value(), ultraRight.get_value());
  if(ultraLeft.get_value() - ultraRight.get_value() > 0){
    //printf("Left bigger. \n");
    //Turn Right
    leftDriveF.move_velocity(rotateVelocity);
    leftDriveR.move_velocity(rotateVelocity);
    rightDriveF.move_velocity(-rotateVelocity);
    rightDriveR.move_velocity(-rotateVelocity);

    while(ultraLeft.get_value()-ultraRight.get_value() > threshold){
      delay(delayTime);
    }
    //Stop
    leftDriveF.move(0);
    leftDriveR.move(0);
    rightDriveF.move(0);
    rightDriveR.move(0);
  }else if(ultraRight.get_value() - ultraLeft.get_value() > 0){
    //printf("Right bigger \n");
    //Turn Left
    leftDriveF.move_velocity(-rotateVelocity);
    leftDriveR.move_velocity(-rotateVelocity);
    rightDriveF.move_velocity(rotateVelocity);
    rightDriveR.move_velocity(rotateVelocity);
    //printf("Movement issued \n");

    while(ultraRight.get_value()-ultraLeft.get_value() > threshold){
      delay(delayTime);
      //printf("Left %d, Right %d \n", ultraLeft.get_value(), ultraRight.get_value());
    }
    //printf("Stopped \n");
    //Stop
    leftDriveF.move(0);
    leftDriveR.move(0);
    rightDriveF.move(0);
    rightDriveR.move(0);
  }

  //printf("Alligned.\n");
}

void Movement::fireAuton(){
  catipult.move_relative(1080, 200);

  while(ABS(catipult.get_position()-catipult.get_target_position()) >= 5){/*Nil*/}
  //Return and continue
}

void Movement::towerPos(int newTarget, int speed){
  towerLeft.move_relative(newTarget, speed);
  towerRight.move_relative(newTarget, speed);
}

void Movement::towerSync(int newTarget, int speed){
  towerPos(newTarget, speed);
  while(ABS(towerRight.get_position()-towerRight.get_target_position()) > 5){/*Whatever*/}
}
