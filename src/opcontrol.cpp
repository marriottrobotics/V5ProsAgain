#include "main.h"

void powerDrive(int leftf, int leftb, int rightf, int rightb);
void fpsControl();
void loaderSpinUp();
void loaderSpinDown();
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

int towerTarget = 0;

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

	//pot.calibrate();

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
    }
}

//Loader Stuff ----------------------------------------------------------------
void updateLoader(){
	//Controller.ButtonX.pressed( loaderSpinUp );
  //Controller.ButtonB.pressed( loaderSpinDown );
	//Event based process not in use.
	/*if(joystick.get_digital_new_press(DIGITAL_X)){
		loaderSpinUp();
	}else if(joystick.get_digital_new_press(DIGITAL_B)){
		loaderSpinDown();
	}*/

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

void loaderSpinDown(){
    if(loaderMode != 1){
        loaderMode = 1;
				loader.move(127);
    }else if(loaderMode == 1){
        loaderMode = 0;
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
//End loader stuff --------------------------------------------------------


//Brake Stuff --------------------------------------------------------------
void updateBrakes(){
	if(joystick.get_digital_new_press(DIGITAL_L1)){
		brake();
	}
}

void brake(){
    if(brakes){
      brakes = false;
    }else{
			leftDriveF.move_absolute(leftDriveF.get_position(), 100);
      leftDriveR.move_absolute(leftDriveR.get_position(), 100);
			rightDriveF.move_absolute(rightDriveF.get_position(), 100);
			rightDriveR.move_absolute(rightDriveR.get_position(), 100);
      brakes = true;
    }
}
//End Brake Stuff -------------------------------------------------------------


//Catipult Stuff --------------------------------------------------------------
void fire(){
	catipult.move_relative(1080, 100);
}

void updateCatipult(){
    if(catipult.get_power() > 1.0 && catipult.is_stopped()){
        //Catipult.stop(vex::brakeType::coast);
    }

		if(joystick.get_digital_new_press(DIGITAL_R1)){
			fire();
		}
}
//End Catipult Stuff ----------------------------------------------------------

//Direction Swapping ----------------------------------------------------------
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

//Tower stuff ------------------------------------------------------------------
void updateTower(){
	//Tower everything
	if(joystick.get_digital(DIGITAL_DOWN)){
		towerTarget+=10;
	}else if(joystick.get_digital(DIGITAL_UP)){
		towerTarget-=10;
	}else if(joystick.get_digital_new_press(DIGITAL_LEFT)){
		towerTarget=1000; //Tower slam on post.
	}else if(joystick.get_digital_new_press(DIGITAL_RIGHT)){
		towerTarget=3000; //Tower down.
	}
	//Now move the motors
	towerLeft.move_absolute(towerTarget, 100);
	towerRight.move_absolute(towerTarget, 100);
}
