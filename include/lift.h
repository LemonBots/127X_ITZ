#include "main.h"
#include "motors.h"
#include "sensors.h"

PD lift = {  //Sets values for lift PD
	.Kp = 2, .Kd = 0.05, .error = 0, .previous_error = 0, .derivative = 0,
	.target = GROUND_CONE, .sensor = 0, .motorvalue = 0
};

void liftHeight(void * parameter){  //liftHeight managing task
	while(1){  //Standard PD control loop
		if(scoringMode != 2){ //If not in manual mode
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

			lcdPrint(LCDSCREEN, 1, "Power: %d", lift.motorvalue);
			lcdPrint(LCDSCREEN, 2, "Switch: %d", digitalRead(MOGOLIMITSWITCH));
			delay(20); //Don't hog CPU!
		} else {  //When in manual mode
			if(joystickGetDigital(1, 7, JOY_UP)){
				lift.motorvalue = 127;
			} else if (joystickGetDigital(1, 7, JOY_DOWN)){
				lift.motorvalue = -127;
			} else {
				lift.motorvalue = 10;
			}
			delay(20);
		}
	}
}
