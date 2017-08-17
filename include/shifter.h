#include "main.h"
#include "motors.h"
#include "sensors.h"

PD shifter = {  //Sets values for shifter PD
	.Kp = 0.27, .Kd = 0.15, .error = 0, .previous_error = 0, .derivative = 0,
	.target = ANGLE_BOTTOM, .sensor = 0, .motorvalue = 0
};

void shifterAngle(void * parameter){  //liftHeight managing task
	while(1){  //Standard PD control loop
		if (scoringMode != 2){
			shifter.sensor = analogRead(SHIFTERPOT);
			shifter.previous_error = shifter.error;
			shifter.error = shifter.sensor - shifter.target;
			shifter.derivative = shifter.previous_error - shifter.error;

			shifter.motorvalue = shifter.Kp * shifter.error + shifter.Kd * shifter.derivative;

			motorSet(CHAINBAR, shifter.motorvalue);
			delay(20);
		} else {
			if(joystickGetDigital(1, 5, JOY_UP)){
				shifter.motorvalue = 127;
			} else if (joystickGetDigital(1, 5, JOY_DOWN)){
				shifter.motorvalue = -127;
			} else {
				shifter.motorvalue = 10;
			}
		}
	}
}
