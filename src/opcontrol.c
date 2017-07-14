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
	.Kp = 0.6, .Kd = 0.00, .error = 0, .previous_error = 0, .derivative = 0,
	.target = GROUND_CONE, .sensor = 0, .motorvalue = 0
};

PD shifter = {  //Sets values for shifter PD
	.Kp = 0.3, .Kd = 0.1, .error = 0, .previous_error = 0, .derivative = 0,
	.target = ANGLE_BOTTOM, .sensor = 0, .motorvalue = 0
};


void liftHeight(void * parameter){  //liftHeight managing task
	while(1){  //Standard PD control loop
		lift.sensor = analogRead(MAINLIFTPOT);
		lift.previous_error = lift.error;
		lift.error = lift.sensor - lift.target;
		lift.derivative = lift.previous_error - lift.error;

		lift.motorvalue = lift.Kp * lift.error + lift.Kd * lift.derivative;
		if(lift.motorvalue > 127){
			lift.motorvalue = 127;
		} else if (lift.motorvalue < -127){
			lift.motorvalue = -127;
		}

		motorSet(MAINLIFT, lift.motorvalue);

		lcdPrint(LCDSCREEN, 1, "Target: %d", lift.target);
		lcdPrint(LCDSCREEN, 2, "Lift: %d", lift.motorvalue);
		delay(20); //Don't hog CPU!
	}
}

void shifterAngle(void * parameter){  //liftHeight managing task
	while(1){  //Standard PD control loop
		shifter.sensor = analogRead(SHIFTERPOT);
		shifter.previous_error = shifter.error;
		shifter.error = shifter.sensor - shifter.target;
		shifter.derivative = shifter.previous_error - shifter.error;

		shifter.motorvalue = shifter.Kp * shifter.error + shifter.Kd * shifter.derivative;

		motorSet(CHAINBAR, shifter.motorvalue);

		delay(20);
	}
}

uint8_t stackedCones = 1; //Cones stacked on currently-selected mobile goal
uint8_t stationaryCones = 0; //Cones stacked on stationary goal (only incremented)
														//in stationary goal mode)
void liftManager(void * parameter){
	while(1){
		while (scoringMode == 0) { //If in standard mogo mode
			printf("Mode: %d", scoringMode);
			printf("Cones: %d\n", stackedCones);
			if(liftPlan == 0){ //If picking up cones from ground
				lift.target = GROUND_CONE;  //Ground cone height
			}
			if(liftPlan == 1){ //If picking up cones from autoloader
				lift.target = LOADER_CONE;  //Autoloader height
			}

			if(liftPlan == 2){  //if plan is to stack
				switch(stackedCones){		//Library for mogo stack heights
					case 1 :							//1 cone is stacked...
					lift.target = 2200;//...this is correct height.
					break;
					case 2 :							//2 cones are stacked...
					lift.target = 2420;		//etc.
					break;
					case 3 :
					lift.target = 2560;
					break;
					case 4 :
					lift.target = 2700;
					break;
					case 5 :
					lift.target = 2900;
					break;
					case 6 :
					lift.target = 3100;
					break;
					case 7 :
					lift.target = 3200;
					break;
					case 8 :
					lift.target = 3400;
					break;
					case 9 :
					lift.target = 2360;
					break;
					case 10 :
					lift.target = 2360;
					break;
					case 11 :
					lift.target = 2360;
					break;
					case 12 :
					lift.target = 2360;
					break;
				}
			}
			delay(20);
		}


		while(scoringMode == 1){ //If in stationary goal mode
			printf("Mode: %d", scoringMode);
			printf("Cones: %d\n", stackedCones);
			if(liftPlan == 0){ //If picking up cones from ground
				lift.target = GROUND_CONE;  //Ground cone height
			}
			if(liftPlan == 1){ //If picking up cones from autoloader
				lift.target = LOADER_CONE;  //Autoloader height
			}

			if(liftPlan == 2){  //If plan is to stack
				//switch stacked cones
			}
			delay(20);
		}
	}
}

void partnerController(void * parameter) {
	while(1){
		if (joystickGetDigital(2, 7, JOY_UP)){ //Sets to mobile goal mode
			scoringMode = 0;
			stackedCones = 0;
			delay(300);
		} else if (joystickGetDigital(2, 7, JOY_DOWN)){ //Sets to stationary goal mode
			scoringMode = 1;
			delay(300);
		} else if (joystickGetDigital(2, 7, JOY_LEFT)){ //Sets to manual control
			scoringMode = 2;
			delay(300);
		}


		if (joystickGetDigital(2, 8, JOY_UP)){ //Sets to autoloader pickup
			pickupMode = 0;
			delay(300);
		} else if (joystickGetDigital(2, 8, JOY_DOWN)){ //Sets to ground pickup
			pickupMode = 1;
			delay(300);
		}

		if(joystickGetDigital(2, 6, JOY_UP)){  //Increments cone counts
			if(scoringMode == 0){					//depending on scoringMode
			stackedCones += 1;
			}	else if (scoringMode ==1){
				stationaryCones += 1;
			}
			delay(300);
		}
		if(joystickGetDigital(2, 6, JOY_DOWN)){  //Decrements cone counts
			if(scoringMode == 0){					//depending on scoringMode
			stackedCones -= 1;
			}	else if (scoringMode ==1){
				stationaryCones -= 1;
			}
			delay(300);
		}

		if(joystickGetDigital(2, 5, JOY_DOWN)){ //Sets stacked cone number to 5
			if(scoringMode == 0){
			stackedCones = 5;  //Change mogo cones count to 5
			} else if (scoringMode == 1){
				stationaryCones = 5;  //Change statgo cones count to 10
			}
			delay(300);
		}
		if(joystickGetDigital(2, 5, JOY_UP)){ //Sets stacked cone number to 10
			if(scoringMode == 0){
			stackedCones = 10;  //Change mogo cones count to 10
		} else if (scoringMode == 1){
			stationaryCones = 10;  //Change statgo cones count to 10
		}
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
		motorSet(DRIVERIGHT1, -rightDrivePower);
		motorSet(DRIVERIGHT2, -rightDrivePower);

		leftDrivePower = deadband(joystickGetAnalog(1, 3));
		motorSet(DRIVELEFT1, -leftDrivePower);
		motorSet(DRIVELEFT2, -leftDrivePower);

		if (joystickGetDigital(1, 5, JOY_UP) && analogRead(MOGOLIMITSWITCH == 1)){
																						//Switch is not pressed
			motorSet(MOGOLIFT, -127);  //raise lift
		} else if (joystickGetDigital(1, 5, JOY_DOWN)){
			motorSet(MOGOLIFT, 127);  //Lower lift
		} else if (analogRead(MOGOLIMITSWITCH == 0)){  //Switch is pressed
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
	taskCreate(shifterAngle,  //Creates task to manage shifter angle
						 	TASK_DEFAULT_STACK_SIZE,
						 	NULL,
						 	TASK_PRIORITY_DEFAULT);
	taskCreate(liftManager,  //Creates task to manage lift target height
							TASK_DEFAULT_STACK_SIZE,
							NULL,
							TASK_PRIORITY_DEFAULT);
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
					delay(200);
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
					delay(50);
					motorSet(ABDUCTOR, 127);  //Close claw again to move
					delay(150);
					shifter.target = ANGLE_BOTTOM; //Move shifter to grabbing height
					while(shifter.sensor > CLEARANCE_ANGLE_TOP){ //Wait until chainbar is
						delay(40); 																//away to go down
					}
					motorSet(ABDUCTOR, 0);
					if (pickupMode == 0){  //If picking up from ground,
						liftPlan = 0;				//go to ground
					} else if (pickupMode == 1){  //If picking up from autoloader,
						liftPlan = 1;				//go to autoloader
					}
					while(shifter.sensor - 50 > shifter.target){  //Wait until shifter is
						delay(40);																	//close enough
					}
					motorSet(ABDUCTOR, -127); //Open claw
					delay(200);
					motorSet(ABDUCTOR, 0);
				}
				delay(20);
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
					delay(50);
					motorSet(ABDUCTOR, 127);
					delay(200);
					motorSet(ABDUCTOR, 0);
					while(shifter.sensor > CLEARANCE_ANGLE_TOP){ //Wait until chainbar is
						delay(40); 																//away to go down
					}
					if (pickupMode == 0){  //If picking up from ground,
						liftPlan = 0;				//go to ground
					} else if (pickupMode ==1){ //If picking up from autoloader,
						liftPlan = 1;				//go to autoloader
					}
					motorSet(ABDUCTOR, -127); //Open claw
					delay(200);
					motorSet(ABDUCTOR, 0);
				}
				delay(20);
			}
		}

		delay(20);
	}
}
