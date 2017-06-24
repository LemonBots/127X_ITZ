#include "main.h"
#include "motors.h"
#include "sensors.h"

int scoringMode = 0;	//0 is standard mobile goal stacking,
											//1 is stationary goal stacking
											//2 is complete manual control

int liftPlan = 0;		//0 indicates going down to pick up from ground;
										//1 indicates going down to pick up from autoloader
										//2 indicates going up to stack

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
		if(liftPlan == 0){ //If picking up cones from ground
			lift.target = 2000;  //Ground cone height
		}
		if(liftPlan == 1){ //If picking up cones from autoloader
			lift.target = 2000;  //Autoloader height
		}

		lift.sensor = analogRead(mainLiftPot);
		lift.previous_error = lift.error;
		lift.error = lift.sensor - lift.target;

		lift.motorvalue = lift.Kp * lift.error + lift.Kd * lift.derivative;

		motorSet(mainLift, lift.motorvalue);
		delay(20);

		lcdPrint(lcdScreen, 1, "Target: %d", lift.target);
		lcdPrint(lcdScreen, 2, "Sensor: %d", lift.sensor);
	}
}

void shifterAngle(void * parameter){  //liftHeight managing task
	while(1){
		shifter.sensor = analogRead(shifterPot);
		shifter.previous_error = shifter.error;
		shifter.error = shifter.sensor - shifter.target;

		shifter.motorvalue = shifter.Kp * shifter.error + shifter.Kd * shifter.derivative;

		motorSet(chainbar, shifter.motorvalue);
		delay(20);
	}
}

int stackedCones = 0;

void liftManager(void * parameter){
	while (scoringMode == 0) { //If in standard mogo mode

		if(liftPlan == 0){  //If picking up from ground....
			lift.target = 2000; //height for picking up goal from ground
		}

		if(liftPlan == 2){  //if goal is to stack
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
	while(1){
		delay(20);
	}
}
