#include "main.h"
#include "motors.h"
#include "sensors.h"

void liftHeight(void * parameter){
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
  TaskHandle liftPID = taskCreate(liftHeight,  //Creates task for DR4B PID
                                  TASK_DEFAULT_STACK_SIZE,
                                  NULL,
                                  TASK_PRIORITY_DEFAULT);
  TaskHandle drivePID = taskCreate(drive,  //Creates task for straightline driving
                                  TASK_DEFAULT_STACK_SIZE,
                                  NULL,
                                  TASK_PRIORITY_DEFAULT);
  TaskHandle gyroPID = taskCreate(gyro,  //Creates task for gyroscopic turning
                                  TASK_DEFAULT_STACK_SIZE,
                                  NULL,
                                  TASK_PRIORITY_DEFAULT);
  taskDelete(liftPID);
  taskDelete(drivePID);
  taskDelete(gyroPID);
}
