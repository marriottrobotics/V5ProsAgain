#include "main.h"

void powerDrive(int leftf, int leftb, int rightf, int rightb);
void fpsControl();
void updateLoader();
void updateBrakes();
void brake();
void fire();
void updateCatipult();
void updateDirection();
void directionSwap();
void updateTower();

int driveMode = 0;
int loaderMode = 0;
int towerMode = 0; //0 means analog movement. 1 means macro movement. 2 means down.

int catJoyLast = 0;

bool direction = false; //False is normal
bool brakes = false;
/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	while (true) {
		fpsControl();
		updateLoader();
		updateBrakes();
		updateDirection();
		updateCatipult();
		updateTower();

		delay(20);
	}
}

void fpsControl(){
		int forward = joystick.get_analog(ANALOG_RIGHT_Y);
    int side = joystick.get_analog(ANALOG_RIGHT_X);
    int turn = joystick.get_analog(ANALOG_LEFT_X);

    int leftf = forward + turn + side;
    int leftb = forward + turn - side;
    int rightf = forward - turn - side;
    int rightb = forward - turn + side;

    if(driveMode == 0){ //Driving Speed --------------
        //Do nothing
    }else if(driveMode == 1){ // 3/4 speed?
        leftf *= 3;
        leftb *= 3;
        rightf *= 3;
        rightb *= 3;

        leftf /= 4;
        leftb /= 4;
        rightf /= 4;
        rightb /= 4;
    }else if(driveMode == 2){ // 1/3 speed?
        leftf /= 3;
        leftb /= 3;
        rightf /= 3;
        rightb /= 3;
    }

    powerDrive(leftf, leftb, rightf, rightb);
}

void powerDrive(int leftf, int leftb, int rightf, int rightb){
	if(!brakes){
        if(!direction){
					leftDriveF.move(leftf);
					leftDriveR.move(leftb);
					rightDriveF.move(rightf);
					rightDriveR.move(rightb);
        }else{
					leftDriveF.move(-rightb);
					leftDriveR.move(-rightf);
					rightDriveF.move(-leftb);
					rightDriveR.move(-leftf);
        }
    }else{
			leftDriveF.move_velocity(0);
			leftDriveR.move_velocity(0);
			rightDriveF.move_velocity(0);
			rightDriveR.move_velocity(0);
		}
}

//Loader Stuff -----------------------------------------------------------------
void updateLoader(){
	//Loader - Also handled by event stuff
	if(joystick.get_digital(DIGITAL_R2)){ //Should load the ball up.
			loader.move(127);
      loaderMode = 0;
  }else if(joystick.get_digital(DIGITAL_L2)){ //Should spit ball out.
			loader.move(-127);
      loaderMode = 0;
  }else if(loaderMode == 0){
			loader.move_absolute(loader.get_position(), 100);
  }
}

void loaderSpinUp(){
    if(loaderMode != 2){
        loaderMode = 2;
				loader.move(-127);
    }else if(loaderMode == 2){
        loaderMode = 0;
        loader.move_absolute(loader.get_position(), 100);
    }
}
//End loader stuff -------------------------------------------------------------


//Brake Stuff ------------------------------------------------------------------
void updateBrakes(){
	if(joystick.get_digital_new_press(DIGITAL_L1)){
		brake();
	}
}

void brake(){
    if(brakes){
      brakes = false;
			leftDriveF.set_brake_mode(MOTOR_BRAKE_COAST);
			leftDriveR.set_brake_mode(MOTOR_BRAKE_COAST);
			rightDriveF.set_brake_mode(MOTOR_BRAKE_COAST);
			rightDriveR.set_brake_mode(MOTOR_BRAKE_COAST);
    }else{
			leftDriveF.set_brake_mode(MOTOR_BRAKE_HOLD);
			leftDriveR.set_brake_mode(MOTOR_BRAKE_HOLD);
			rightDriveF.set_brake_mode(MOTOR_BRAKE_HOLD);
			rightDriveR.set_brake_mode(MOTOR_BRAKE_HOLD);
      brakes = true;
    }
}
//End Brake Stuff --------------------------------------------------------------

//Catipult Stuff ---------------------------------------------------------------
void fire(){
	catipult.move_relative(1080, 200);
}

void updateCatipult(){
	//printf("Catipult Voltate %d \n", catipult.get_voltage());
	//Automated kill switch. Not sure if it works.
	if(catipult.get_voltage() >= 200 && catipult.is_stopped()){
		pros::lcd::set_text(0, "WATCHDOG!!!");
		//catipult.move(0);
	}

	//Manual Kill Switch.
	if(joystick.get_digital_new_press(DIGITAL_X) || joystick.get_analog(ANALOG_RIGHT_Y) - catJoyLast > 100){
		catipult.move(0);
	}
	catJoyLast = joystick.get_analog(ANALOG_RIGHT_Y);

	if(joystick.get_digital_new_press(DIGITAL_R1) || joystick.get_digital_new_press(DIGITAL_Y)){
		fire();
	}
}
//End Catipult Stuff -----------------------------------------------------------

//Direction Swapping -----------------------------------------------------------
void updateDirection(){
		if(joystick.get_digital_new_press(DIGITAL_A)){
			directionSwap();
		}
}

void directionSwap(){
    if(direction == true){
        direction = false;
    }else if(direction == false){
        direction = true;
    }
}
//End Direction Swapping -------------------------------------------------------

//Tower Stuff ------------------------------------------------------------------
void updateTower(){
	//Tower everything
	if(joystick.get_digital(DIGITAL_LEFT) && towerLimit.get_value() == false){
		towerMode = 0;
		towerLeft.move(200);
		towerRight.move(200);
	}else if(joystick.get_digital(DIGITAL_RIGHT) && towerLeft.get_position() > 0 && towerRight.get_position() > 0){
		towerMode = 0;
		towerLeft.move(-150);
		towerRight.move(-150);
	}else if(joystick.get_digital_new_press(DIGITAL_DOWN)){
		towerMode = 2;
		//towerLeft.move_absolute(1195, 200);
		//towerRight.move_absolute(1195, 200);
		towerLeft.move(200);
		towerRight.move(200);
	}else if(joystick.get_digital_new_press(DIGITAL_UP)){
		towerMode = 1;
		towerLeft.move_absolute(355, 200);
		towerRight.move_absolute(355, 200);
	}else if(joystick.get_digital_new_press(DIGITAL_B)){
		towerMode = 1;
		towerRight.move_absolute(870, 200);
		towerLeft.move_absolute(870, 200);
	}else if(towerMode == 0){
		towerLeft.move_velocity(0);
		towerRight.move_velocity(0);
	}

	if(towerMode == 2 && towerLimit.get_value() == true){
		towerLeft.move_velocity(0);
		towerRight.move_velocity(0);
		towerMode = 0;
	}
}
//End Tower Stuff --------------------------------------------------------------
