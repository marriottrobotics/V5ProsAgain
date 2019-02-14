#include "main.h"
#include "Movements.h"

#define BLUE false
#define RED true
#define TOP true
#define BOTTOM false

bool red = false;
bool top = false;

void autonTop();
void autonBottom();
void brakesOn();

using namespace Movement;

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
  configureAuton(); //Make sure this is correct.

  brakesOn();

  if(Movement::top){
    autonTop();
  }else{
    autonBottom();
  }
}

void configureAuton(){
  if(jRed.get_value() == HIGH){
    Movement::red = RED;
  }else{
    Movement::red = BLUE;
  }

  if(jTop.get_value() == HIGH){
    Movement::top = TOP;
  }else{
    Movement::top = BOTTOM;
  }

  if(Movement::red && Movement::top){
    pros::lcd::print(0, "Red Top");
  }else if(Movement::red && !Movement::top){
    pros::lcd::print(0, "Red Bottom");
  }else if(!Movement::red && Movement::top){
    pros::lcd::print(0, "Blue Top");
  }else if(!Movement::red && !Movement::top){
    pros::lcd::print(0, "Blue Bottom");
  }
}

void autonTop(){
   loader.move(127);

   powerMotor(100);

   printf("Movement started \n");
   pros::lcd::print(0, "Movement started");
   while(lineRight.get_value() < 2900){
     //printf("Line sensor = %d \n", lineLeft.get_value());
   }
   printf("Controlled movement. \n");
   pros::lcd::print(1, "ControlledMovement");
   moveAccel(900); //drive(900, 150);
   loader.move_relative(360, 200);

  drive(-1200, 150);
  turnUp(630, 150); //Then coordinate with ultrasonic sensors?
  printf("\n Turn up completed. \n");

  pros::lcd::print(1, "Flag 1");

  //Testing new code
  //alignUltrasonic();
  //distUltrasonic(1140);
  //End test block

  //Drive to the first flag

  powerMotor(50);

  //Wait for ultra
  while(ultraRight.get_value() > 1140){/*Nada*/}

  powerMotor(0);

  fireAuton();

  pros::lcd::print(2, "Flag 2");
  loader.move_velocity(127);
  //Drive to the second flag
  powerMotor(75);

  //Wait for ultra
  while(ultraLeft.get_value() > 550){/*Nada*/}

  powerMotor(0);

  //Testing new code
  //alignUltrasonic();
  //distUltrasonic(550);

  loader.move_relative(720, 100);

  fireAuton();

  slideUp(100, 200);
  moveAccel(500); //drive(500, 200);

  drive(-300, 200);
  loader.move_velocity(-150);
/*
  drive(-1000, 200);
  turnUp(-600, 200);
  drive(1000, 200);

  turnUp(100, 200);
  drive(500, 200);*/
}

void autonBottom(){
  printf("Tower start \n");
  towerLeft.move_velocity(150);
  towerRight.move_velocity(150);
  while(towerLimit.get_value() != 1) {delay(1);}
  towerLeft.move_relative(50, 50);
  towerRight.move_relative(50, 50);
  printf("Tower Stop \n");
  powerMotor(-100);

  printf("Movement started \n");
  pros::lcd::print(0, "Movement started");
  while(lineRight.get_value() < 2900){
    //printf("Line sensor = %d \n", lineLeft.get_value());
  }
  printf("Controlled movement. \n");
  pros::lcd::print(1, "ControlledMovement");
  drive(-700, 200);

  //slideUp(350, 200);
  drive(500, 200);
  printf("Slide \n");
  slideUp(800, 200);
  alignTime(5, 1000, 1);
  printf("Alligned \n");
  drive(-500, 50);
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
  towerSync(-300, 200);
  slideUp(575, 200);
  alignTime(5, 250, 1);
  printf("Park? \n");

  drive(-1500, 200);
  /*towerSync(1100, 100);

  powerMotor(-100);

  printf("Movement started \n");
  pros::lcd::print(0, "Movement started");
  while(lineRight.get_value() < 2900){
    //printf("Line sensor = %d \n", lineLeft.get_value());
  }
  printf("Controlled movement. \n");
  pros::lcd::print(1, "ControlledMovement");

  drive(-750, 150);
  slideUp(400, 200);
  drive(400, 150);
  slideUp(400, 300);
  drive(-350, 150);
  towerSync(-300, 100);*/
}

void brakesOn(){
  leftDriveF.set_brake_mode(MOTOR_BRAKE_HOLD);
  leftDriveR.set_brake_mode(MOTOR_BRAKE_HOLD);
  rightDriveF.set_brake_mode(MOTOR_BRAKE_HOLD);
  rightDriveR.set_brake_mode(MOTOR_BRAKE_HOLD);
}
