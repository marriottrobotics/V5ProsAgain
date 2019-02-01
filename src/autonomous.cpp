#include "main.h"

#define BLUE false
#define RED true
#define TOP true
#define BOTTOM false

bool red = false;
bool top = false;

void autonTop();
void autonBottom();

void powerMotor(int speed);
void drive(int distance, int speed);
void driveAsync(int distance, int speed);
void turnUp(int distance, int speed);
void slideUp(int distance, int speed);
void alignUltrasonic();
void fireAuton();
void towerPos(int newTarget, int speed);
void towerSync(int newTarget, int speed);

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

  towerRight.move_absolute(-360, 100);

  if(top){
    autonTop();
  }else{
    autonBottom();
  }
}

void configureAuton(){
  if(jRed.get_value() == HIGH){
    red = RED;
  }else{
    red = BLUE;
  }

  if(jTop.get_value() == HIGH){
    top = TOP;
  }else{
    top = BOTTOM;
  }

  if(red && top){
    pros::lcd::print(0, "Red Top");
  }else if(red && !top){
    pros::lcd::print(0, "Red Bottom");
  }else if(!red && top){
    pros::lcd::print(0, "Blue Top");
  }else if(!red && !top){
    pros::lcd::print(0, "Blue Bottom");
  }
}

void autonTop(){
   loader.move(127);

   powerMotor(100);

   printf("Movement started \n");
   pros::lcd::print(0, "Movement started");
   while(lineLeft.get_value() < 2900){
     //printf("Line sensor = %d \n", lineLeft.get_value());
   }
   printf("Controlled movement. \n");
   pros::lcd::print(1, "ControlledMovement");
   drive(900, 150);
   loader.move_relative(360, 200);

  drive(-1325, 150);
  turnUp(650, 150); //Then coordinate with ultrasonic sensors?
  printf("\n Turn up completed. \n");

  pros::lcd::print(1, "Flag 1");
  //Drive to the first flag
  powerMotor(50);

  //Wait for ultra
  while(ultraLeft.get_value() > 1140){/*Nada*/}

  powerMotor(0);

  fireAuton();

  pros::lcd::print(2, "Flag 2");
  loader.move_velocity(127);
  //Drive to the second flag
  powerMotor(75);

  //Wait for ultra
  while(ultraLeft.get_value() > 550){/*Nada*/}

  powerMotor(0);

  loader.move_relative(720, 100);

  fireAuton();

  slideUp(50, 200);
  drive(600, 200);

  loader.move_velocity(-150);

  drive(-1000, 200);
  turnUp(-600, 200);
  drive(1000, 200);

  turnUp(100, 200);
  drive(500, 200);
}

void autonBottom(){
  towerSync(1100, 100);

  powerMotor(-100);

  printf("Movement started \n");
  pros::lcd::print(0, "Movement started");
  while(lineLeft.get_value() < 2900){
    //printf("Line sensor = %d \n", lineLeft.get_value());
  }
  printf("Controlled movement. \n");
  pros::lcd::print(1, "ControlledMovement");

  drive(-750, 150);
  slideUp(400, 200);
  drive(400, 150);
  slideUp(400, 300);
  drive(-350, 150);
  towerSync(-300, 100);
}

void powerMotor(int speed){
  leftDriveF.move_velocity(speed);
  leftDriveR.move_velocity(speed);
  rightDriveF.move_velocity(speed);
  rightDriveR.move_velocity(speed);
}

void drive(int distance, int speed){
  leftDriveF.move_relative(distance, speed);
  leftDriveR.move_relative(distance, speed);
  rightDriveF.move_relative(distance, speed);
  rightDriveR.move_relative(distance, speed);
  while(ABS(rightDriveR.get_position()-rightDriveR.get_target_position()) >= 10){
    //Do nothing
  }
}

void driveAsync(int distance, int speed){
  leftDriveF.move_relative(distance, speed);
  leftDriveR.move_relative(distance, speed);
  rightDriveF.move_relative(distance, speed);
  rightDriveR.move_relative(distance, speed);
}

void turnUp(int distance, int speed){
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

void slideUp(int distance, int speed){
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

void alignUltrasonic(){
  if(ultraLeft.get_value() - ultraRight.get_value() > 5){
    //Turn Right
    leftDriveF.move(-25);
    leftDriveR.move(-25);
    rightDriveF.move(25);
    rightDriveR.move(25);

    while(ultraLeft.get_value()-ultraRight.get_value() > 5){
      delay(20);
    }
    //Stop
    leftDriveF.move(0);
    leftDriveR.move(0);
    rightDriveF.move(0);
    rightDriveR.move(0);
  }else if(ultraRight.get_value() - ultraLeft.get_value() > 5){
    //Turn Left
    leftDriveF.move(25);
    leftDriveR.move(25);
    rightDriveF.move(-25);
    rightDriveR.move(-25);
    while(ultraLeft.get_value()-ultraRight.get_value() > 5){
      delay(20);
    }
    //Stop
    leftDriveF.move(0);
    leftDriveR.move(0);
    rightDriveF.move(0);
    rightDriveR.move(0);
  }
}

void fireAuton(){
  catipult.move_relative(1080, 200);

  while(ABS(catipult.get_position()-catipult.get_target_position()) >= 5){/*Nil*/}
  //Return and continue
}

void towerPos(int newTarget, int speed){
  towerLeft.move_relative(newTarget, speed);
  towerRight.move_relative(newTarget, speed);
}

void towerSync(int newTarget, int speed){
  towerPos(newTarget, speed);
  while(ABS(towerRight.get_position()-towerRight.get_target_position()) > 5){/*Whatever*/}
}
