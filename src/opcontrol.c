#include "main.h"
#include "motors.h"
#include "sensors.h"

void liftHeight(void * parameter){

}

void operatorControl() {
	TaskHandle liftPID = taskCreate(liftHeight,  //"unused variable 'liftPID' [-Wunused-variable]"
                                  TASK_DEFAULT_STACK_SIZE,
                                  NULL,
                                  TASK_PRIORITY_DEFAULT);
	while (1) {
		delay(20);
	}
}
