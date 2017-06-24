#include "main.h"
#ifndef SENSORS_H_
#define SENSORS_H_

//Drive Quadrature Encoders (digital)
#define leftDriveQuadTop 1
#define leftDriveQuadBottom 2

#define rightDriveQuadTop 3
#define rightDriveQuadBottom 4

//Main Lift Potentiometer (analog)
#define mainLiftPot 1

//Shifter Potentiometer
#define shifterPot 2

//Mogo Lift Limit Switch (analog)
#define mogoLiftSwitch 3

//Drive Gyroscope

#define gyroscope 3

//LCD screen

#define lcdScreen uart1

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

#endif
