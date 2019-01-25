#include "main.h"

bool red = false;
bool top = false;

void autonTop();
void autonBottom();

void drive(int distance, int speed);
void driveAsync(int distance, int speed);
void turnUp(int distance, int speed);
void slideUp(int distance, int speed);
void alignUltrasonic();
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

  if(top){
    autonTop();
  }else{
    autonBottom();
  }
}

void configureAuton(){
  if(jRed.get_value() == HIGH){
    red = true;
  }else{
    red = false;
  }

  if(jTop.get_value() == HIGH){
    top = true;
  }else{
    top = false;
  }

  if(red && top){
    pros::lcd::set_text(0, "Red Top");
  }else if(red && !top){
    pros::lcd::set_text(0, "Red Bottom");
  }else if(!red && top){
    pros::lcd::set_text(0, "Blue Top");
  }else if(!red && !top){
    pros::lcd::set_text(0, "Blue Bottom");
  }
}

void autonTop(){
   loader.move(-127);
   drive(1115, 50);
   loader.move_relative(-1080, 100);

  drive(-1000, 50); //Drive until line sensor
  turnUp(650, 50); //Then coordinate with ultrasonic sensors
  alignUltrasonic();
  drive(300, 50);
  catipult.move_relative(1080, 100);
  loader.move(-127);
  drive(500, 50); //Then check ultrasonic distance
  loader.move_relative(-1440, -100);
  catipult.move_relative(1080, 100);
  slideUp(170, 100);
  drive(600, 50);
}

void autonBottom(){
  loader.move(-127);
  drive(1115, 50);
  loader.move_relative(-1080, 100);

  drive(-400, 50);
  loader.move(127);

  slideUp(-675, 100);

  //Loader.stop();
  drive(450, 25);
  drive(-1155, 50);
  loader.move(0);
  slideUp(1650, 100);
  drive(1500, 100);
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
