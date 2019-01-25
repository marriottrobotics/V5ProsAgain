#include "main.h"
using namespace pros;

Controller joystick (CONTROLLER_MASTER);

Motor leftDriveF (3, MOTOR_DEF);
Motor leftDriveR (11, MOTOR_DEF);
Motor rightDriveF (10, MOTOR_REV);
Motor rightDriveR (19, MOTOR_REV);

Motor loader (1, MOTOR_DEF);
Motor catipult (20, MOTOR_REV);

Motor towerLeft (12, MOTOR_DEF);
Motor towerRight (9, MOTOR_REV);

ADIDigitalIn jRed (1);
ADIDigitalIn jTop (2);

ADIUltrasonic ultraLeft (3, 4);
ADIUltrasonic ultraRight (5, 6);

ADILineSensor lineRight (7);
ADILineSensor lineLeft(8);

void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Everything Running ish.");

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
	while(true){
		configureAuton();
		delay(50);
	}
}
