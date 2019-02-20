#include "skillsauton.hpp"

#include "main.h"
#include "Movements.h"

using namespace Movement;

  void skills::skillsAuton(){
    printf("Tower start \n");
    towerDown();
    printf("Tower Stop \n");
    powerMotor(-100);
    printf("Movement started \n");
    pros::lcd::print(0, "Movement started");
    while(lineRight.get_value() < 2900){
      //printf("Line sensor = %d \n", lineLeft.get_value());
    }
    printf("Controlled movement. \n");
    pros::lcd::print(1, "ControlledMovement");
    drive(-650, 150);

    towerSync(-300, 200);

    towerDown();

    //slideUp(350, 200);
    drive(500, 200);
    printf("Slide \n");
    slideUp(750, 200);
    alignTime(5, 750, 1);
    printf("Alligned \n");
    drive(-450, 100);
    towerSync(-300, 50);
    printf("Grabbed \n");
    distUltrasonic(700);

    turnUp(666, 50);
    printf("Turned \n");
    alignTime(5, 500, 1);
    printf("Aligned \n");

    distUltrasonic(44);

    towerSync(-500, 200);
    drive(-400, 200);
    towerDown();
    //End firt cap.
    //Second cap
    distUltrasonic(170);
    turnUp(-630, 150);
    alignTime(5, 1000, 1);
    printf("End bottom auton \n");

    drive(-1000, 200);
    towerSync(-300, 150);
    drive(-1100, 150);
    turnUp(630, 150);
    alignTime(5, 1000, 1);
    distUltrasonic(44);
    towerSync(-500, 200);
    drive(-400, 200);
    towerLeft.move_absolute(0, 150);
    towerRight.move_absolute(0, 150);
    //End Second Cap.

    //Flag?
    distUltrasonic(763);
    turnUp(-630, 150);
    loader.move_velocity(-150);
    drive(500, 200);
    loader.move_relative(1080, -200);
    drive(-1000, 200);

    //Turn to Flag
    turnUp(630, 150);
    drive(1000, 200);
    
  }
