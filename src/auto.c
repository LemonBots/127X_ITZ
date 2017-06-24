#include "main.h"
#include "motors.h"
#include "sensors.h"

void liftHeightAuto(void * parameter){
  while(1){

  }
}

void drive(void * parameter){
  while(1){

  }
}

void gyro(void * parameter){
  while(1){

  }
}

void autonomous() {
  TaskHandle liftPD = taskCreate(liftHeightAuto,  //Creates task for DR4B PD
                                  TASK_DEFAULT_STACK_SIZE,
                                  NULL,
                                  TASK_PRIORITY_DEFAULT);
  TaskHandle driveP = taskCreate(drive,  //Creates task for straightline driving
                                  TASK_DEFAULT_STACK_SIZE,
                                  NULL,
                                  TASK_PRIORITY_DEFAULT);
  TaskHandle gyroPD = taskCreate(gyro,  //Creates task for gyroscopic turning
                                  TASK_DEFAULT_STACK_SIZE,
                                  NULL,
                                  TASK_PRIORITY_DEFAULT);
  taskDelete(liftPD);
  taskDelete(driveP);
  taskDelete(gyroPD);
}
