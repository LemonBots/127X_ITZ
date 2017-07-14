#include "main.h"
#ifndef SENSORS_H_
#define SENSORS_H_

//Drive Quadrature Encoders (digital)
#define LEFTDRIVETOP 1
#define LEFTDRIVEBOTTOM 2

#define RIGHTDRIVETOP 3
#define RIGHTDRIVEBOTTOM 4

//Mogo Lift Limit Switch (digital)
#define MOGOLIMITSWITCH 5

//Main Lift Potentiometer (analog)
#define MAINLIFTPOT 1

//Shifter Potentiometer (analog)
#define SHIFTERPOT 2

//Drive Gyroscope (analog)

#define GYROSCOPE 3

//LCD screen

#define LCDSCREEN uart1

//PD structs

typedef struct NAME{
  double Kp; //Constant for proportional value
  double Kd; //Constant for derivative value
  double error;  //Distance between target and sensor value
  double previous_error;  //Last error value given
  double derivative;  //error - previous_error
  int target;  //Target value
  int sensor;  //Sensor value
  int motorvalue; //Ultimate output
}PD;

#define GROUND_CONE 1900 //Appropriate lift height for a ground cone
                          //Technically 1900, but this gives passive downward power

#define LOADER_CONE 2500 //Appropriate lift height for a loader cone

#define ANGLE_BOTTOM 220 //Angle where shifter is ready to pick up

#define STACK_ANGLE 3700 //Angle where cone is directly above stack

#define MIDDLE_ANGLE 2170 //Angle where cone is right above robot, waiting to stack

#define CLEARANCE_ANGLE_BOTTOM 2000 //Angle that shifter has to get to before
                          //it is clear to go up

#define CLEARANCE_ANGLE_TOP 2000 //Angle that shifter has to get to before
                          //it is clear to go back down

#endif
