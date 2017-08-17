#include "main.h"
#include "motors.h"
#include "sensors.h"
#include "shifter.h"
#include "lift.h"
#include "partnerController.h"
#include "deadband.h"
#include <stdint.h>

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
					lift.target = 2000;//...this is correct height.
					break;
					case 2 :							//2 cones are stacked...
					lift.target = 2110;		//etc.
					break;
					case 3 :
					lift.target = 2210;
					break;
					case 4 :
					lift.target = 2320;
					break;
					case 5 :
					lift.target = 2420;
					break;
					case 6 :
					lift.target = 2530;
					break;
					case 7 :
					lift.target = 2630;
					break;
					case 8 :
					lift.target = 2740;
					break;
					case 9 :
					lift.target = 2840;
					break;
					case 10 :
					lift.target = 2950;
					break;
					case 11 :
					lift.target = 3050;
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

		if (joystickGetDigital(1, 5, JOY_UP) && digitalRead(MOGOLIMITSWITCH) == 1){  //Switch is not pressed
			motorSet(MOGOLIFT, -127);  //raise lift
		} else if (joystickGetDigital(1, 5, JOY_DOWN)){
			motorSet(MOGOLIFT, 127);  //Lower lift
		} else if (digitalRead(MOGOLIMITSWITCH == 0)){  //Switch is pressed
			motorSet(MOGOLIFT, -15); //Passive power
		} else {
			motorSet(MOGOLIFT, 0);
		}

		delay(20);
	}
}

bool manualHoldMode = 0;

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
					delay(100);
					shifter.target = CLEARANCE_ANGLE_BOTTOM + 200; //Move shifter to grabbing height
					while(shifter.sensor > CLEARANCE_ANGLE_TOP){ //Wait until chainbar is
						delay(40); 																//away to go down
					}
					motorSet(ABDUCTOR, 0);
					if (pickupMode == 0){  //If picking up from ground,
						liftPlan = 0;				//go to ground
					} else if (pickupMode == 1){  //If picking up from autoloader,
						liftPlan = 1;				//go to autoloader
					}
					while(lift.error > 50){
						delay(40);
					}
					shifter.target = ANGLE_BOTTOM;
					while(shifter.sensor - 100 > shifter.target){  //Wait until shifter is
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
		if(scoringMode ==2){
			if (joystickGetDigital(1, 6, JOY_UP))
				motorSet(ABDUCTOR, 127);
				manualHoldMode = 1;
			} else if (joystickGetDigital(1, 6, JOY_DOWN)){
				motorSet(ABDUCTOR, -127);
				manualHoldMode = 0;
			} else if (manualHoldMode ==1){
				motorSet(ABDUCTOR, 15);
			} else {
				motorSet(ABDUCTOR, 0);
			}

		delay(20);
	}
}
