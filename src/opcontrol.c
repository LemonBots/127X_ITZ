#include "main.h"
#include "motors.h"
#include "sensors.h"
#include <stdint.h>

uint8_t scoringMode = 0;	//0 is standard mobile goal stacking,
											//1 is stationary goal stacking
											//2 is complete manual control
											//Accepts partner controller input

uint8_t liftPlan = 0;		//0 indicates going down to pick up from ground;
										//1 indicates going down to pick up from autoloader
										//2 indicates going up to stack
										//Does NOT accept partner controller input (backend)

bool pickupMode = 0;  //0 indicates picking up from ground
										//1 indicates picking up from autoloader
										//Accepts partner controller input

PD lift = {  //Sets values for lift PD
	.Kp = 0.1, .Kd = 0.02, .error = 0, .previous_error = 0, .derivative = 0,
	.target = 1500, .sensor = 0, .motorvalue = 0
};

PD shifter = {  //Sets values for shifter PD
	.Kp = 0.1, .Kd = 0.02, .error = 0, .previous_error = 0, .derivative = 0,
	.target = 1500, .sensor = 0, .motorvalue = 0
};


void liftHeight(void * parameter){  //liftHeight managing task
	while(1){
		lift.sensor = analogRead(MAINLIFTPOT);
		lift.previous_error = lift.error;
		lift.error = lift.sensor - lift.target;

		lift.motorvalue = lift.Kp * lift.error + lift.Kd * lift.derivative;

		motorSet(MAINLIFT, lift.motorvalue);

		lcdPrint(LCDSCREEN, 2, "Lift: %d", lift.sensor);
		delay(20); //Don't hog CPU!
	}
}

void shifterAngle(void * parameter){  //liftHeight managing task
	while(1){
		shifter.sensor = analogRead(SHIFTERPOT);
		shifter.previous_error = shifter.error;
		shifter.error = shifter.sensor - shifter.target;

		shifter.motorvalue = shifter.Kp * shifter.error + shifter.Kd * shifter.derivative;

		motorSet(CHAINBAR, shifter.motorvalue);

		lcdPrint(LCDSCREEN, 1, "Arm: %d", shifter.target);
		delay(20);
	}
}

int stackedCones = 0;

void liftManager(void * parameter){
	while (scoringMode == 0) { //If in standard mogo mode

		if(liftPlan == 0){ //If picking up cones from ground
			lift.target = GROUND_CONE;  //Ground cone height
		}
		if(liftPlan == 1){ //If picking up cones from autoloader
			lift.target = LOADER_CONE;  //Autoloader height
		}

		if(liftPlan == 2){  //if plan is to stack
			switch(stackedCones){		//Library for mogo stack heights
				case 1 :							//1 cone is stacked...
				lift.target = 2000;		//...this is correct height.
				case 2 :							//2 cones are stacked...
				lift.target = 2000;		//etc.
				case 3 :
				lift.target = 2000;
				case 4 :
				lift.target = 2000;
				case 5 :
				lift.target = 2000;
				case 6 :
				lift.target = 2000;
				case 7 :
				lift.target = 2000;
				case 8 :
				lift.target = 2000;
				case 9 :
				lift.target = 2000;
				case 10 :
				lift.target = 2000;
				case 11 :
				lift.target = 2000;
				case 12 :
				lift.target = 2000;
				case 13 :
				lift.target = 2000;
				case 14 :
				lift.target = 2000;
				case 15 :
				lift.target = 2000;
				case 16 :
				lift.target = 2000;
				case 17 :
				lift.target = 2000;
				case 18 :
				lift.target = 2000;
			}
		}
		delay(20);
	}


	while(scoringMode == 1){ //If in stationary goal mode

		if(liftPlan == 0){ //If picking up cones from ground
			lift.target = GROUND_CONE;  //Ground cone height
		}
		if(liftPlan == 1){ //If picking up cones from autoloader
			lift.target = LOADER_CONE;  //Autoloader height
		}

		if(liftPlan == 2){  //If plan is to stack
			switch(stackedCones){		//Library for stationary stack heights
				case 1 :							//1 cone is stacked...
				lift.target = 2000;		//...this is correct height.
				case 2 :							//2 cones are stacked...
				lift.target = 2000;		//etc.
				case 3 :
				lift.target = 2000;
				case 4 :
				lift.target = 2000;
				case 5 :
				lift.target = 2000;
				case 6 :
				lift.target = 2000;
				case 7 :
				lift.target = 2000;
				case 8 :
				lift.target = 2000;
				case 9 :
				lift.target = 2000;
				case 10 :
				lift.target = 2000;
				case 11 :
				lift.target = 2000;
				case 12 :
				lift.target = 2000;
			}
		}
	}
	delay(20);
}

void partnerController(void * parameter) {
	while(1){
		if (joystickGetDigital(2, 7, JOY_UP)){ //Sets to mobile goal mode
			scoringMode = 0;
			stackedCones = 0;
			delay(300);
		}
		if (joystickGetDigital(2, 7, JOY_DOWN)){ //Sets to stationary goal mode
			scoringMode = 1;
			stackedCones = 0;
			delay(300);
		}
		if (joystickGetDigital(2, 7, JOY_LEFT)){ //Sets to manual control
			scoringMode = 2;
			delay(300);
		}


		if (joystickGetDigital(2, 8, JOY_UP)){ //Sets to autoloader pickup
			pickupMode = 0;
			delay(300);
		}
		if (joystickGetDigital(2, 8, JOY_DOWN)){ //Sets to ground pickup
			pickupMode = 1;
			delay(300);
		}

		if(joystickGetDigital(2, 6, JOY_UP)){
			stackedCones += 1;
			delay(300);
		}
		if(joystickGetDigital(2, 6, JOY_DOWN)){
			stackedCones -= 1;
		}

		if(joystickGetDigital(2, 5, JOY_DOWN)){ //Sets stacked cone number to 5
			stackedCones = 5;
			delay(300);
		}
		if(joystickGetDigital(2, 5, JOY_UP)){ //Sets stacked cone number to 10
			stackedCones = 10;
			delay(300);
		}

		delay(20);
	}
}

int8_t deadband(int8_t value){  //Does not return a small motor value
	if(abs(value) < 15){
		return 0;
	} else {
		return value;
	}
}

int8_t leftDrivePower;
int8_t rightDrivePower;

void driveTask(void * parameter){
	while(1){
		rightDrivePower = deadband(joystickGetAnalog(1, 2));
		motorSet(DRIVERIGHT1, rightDrivePower);
		motorSet(DRIVERIGHT2, rightDrivePower);

		leftDrivePower = deadband(joystickGetAnalog(1, 3));
		motorSet(DRIVELEFT1, leftDrivePower);
		motorSet(DRIVELEFT2, leftDrivePower);

		if (joystickGetDigital(1, 5, JOY_UP) && analogRead(MOGOLIMITSWITCH == 0)){
			motorSet(MOGOLIFT, -127);  //raise lift
		} else if (joystickGetDigital(1, 5, JOY_DOWN)){
			motorSet(MOGOLIFT, 127);  //Lower lift
		} else if (analogRead(MOGOLIMITSWITCH)){
			motorSet(MOGOLIFT, -15); //Passive power
		} else {
			motorSet(MOGOLIFT, 0);
		}

		delay(20);
	}
}

void operatorControl() {
	taskCreate(liftHeight,  //Creates task to manage lift height
            	TASK_DEFAULT_STACK_SIZE,
            	NULL,
          		TASK_PRIORITY_DEFAULT);
	taskCreate(shifterAngle,  //Creates task to manage lift height
						 	TASK_DEFAULT_STACK_SIZE,
						 	NULL,
						 	TASK_PRIORITY_DEFAULT);
	taskCreate(liftManager,  //Creates task to manage lift target height
							TASK_DEFAULT_STACK_SIZE,
							NULL,
							TASK_PRIORITY_LOWEST);
	taskCreate(partnerController,  //Creates task to take input from partner controller
							TASK_DEFAULT_STACK_SIZE,
							NULL,
							TASK_PRIORITY_DEFAULT);
	taskCreate(driveTask,  //Creates task to take input from partner controller
							TASK_DEFAULT_STACK_SIZE,
							NULL,
							TASK_PRIORITY_DEFAULT);
	while(1){
		if(scoringMode == 0){  //For scoring on mobile goals
			while ((joystickGetAnalog(1, 4) > -100) && (joystickGetAnalog(1, 1) < 100)){
				//Gives a way to cancel (pushing joysticks outward simultaneously)
				if (joystickGetDigital(1, 6, JOY_UP)){
					motorSet(ABDUCTOR, 127); //Close claw
					delay(200);
					motorSet(ABDUCTOR, 15); //Passive power
					delay(100);
				 	shifter.target = MIDDLE_ANGLE; //Raise cone to above
					while (shifter.sensor < CLEARANCE_ANGLE_BOTTOM){ //Wait until abductor is clear
						delay(40);
					}
					liftPlan = 2;  //Lift up
					while (lift.sensor < lift.target - 100){ //Wait until cone is high enough
						delay(40);
					}
					shifter.target = STACK_ANGLE; //Bring to angle for stacking
					while(shifter.sensor + 50 < STACK_ANGLE){ //Wait until shifter is close enough
						delay(40);
					}
					delay(200); //Wait for shifter to settle
					motorSet(ABDUCTOR, -127); //Open claw
					delay(200);
					motorSet(ABDUCTOR, 0); //Turns off motor
					shifter.target = ANGLE_BOTTOM; //Move claw to grabbing height
					while(shifter.sensor > CLEARANCE_ANGLE_TOP){ //Wait until chainbar is
						delay(40); 																//away to go down
					}
					if (pickupMode == 0){
						liftPlan = 0;
					} else if (pickupMode ==1){
						liftPlan = 1;
					}
				}
				break; //Breaks while loop without manual cancel
			}
		}

		if(scoringMode ==1){  //For scoring on stationary goals
			while ((joystickGetAnalog(1, 4) > -100) && (joystickGetAnalog(1, 1) < 100)){
				//Gives a way to cancel (pressing joysticks outward simultaneously)
				if (joystickGetDigital(1, 6, JOY_UP)){
					motorSet(ABDUCTOR, 127); //Close claw
					delay(200);
					motorSet(ABDUCTOR, 15); //Passive power
					delay(100);
				 	shifter.target = MIDDLE_ANGLE; //Raise cone to above
					while (shifter.sensor < CLEARANCE_ANGLE_BOTTOM){ //Wait until abductor is clear
						delay(40);
					}
					liftPlan = 2;  //Lift up
					while (joystickGetDigital(1, 6, JOY_DOWN) == 0){ //Wait for button input to score
						delay(40);
					}
					shifter.target = STACK_ANGLE; //Bring to angle for stacking
					while(shifter.sensor + 50 < STACK_ANGLE){ //Wait until shifter is close enough
						delay(40);
					}
					delay(200); //Wait for shifter to settle
					motorSet(ABDUCTOR, -127); //Open claw
					delay(200);
					motorSet(ABDUCTOR, 0); //Turns off motor
					shifter.target = ANGLE_BOTTOM; //Move claw to grabbing height
					while(shifter.sensor > CLEARANCE_ANGLE_TOP){ //Wait until chainbar is
						delay(40); 																//away to go down
					}
					if (pickupMode == 0){
						liftPlan = 0;
					} else if (pickupMode ==1){
						liftPlan = 1;
					}
				}
				break;  //Breaks while loop without manual cancel
			}
		}

		delay(20);
	}
}
