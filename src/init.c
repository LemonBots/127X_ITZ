#include "main.h"
#include "sensors.h"

#define mainPage 1 //Front page of LCD screen
#define maxPages 8 //Highest page number

void initializeIO() {
}

int lcdPageNumber = mainPage;
int autonMode = 0;
double batteryVoltage;

void autonSelect(){
  while(!isEnabled()){
    batteryVoltage = powerLevelMain() / 10;

    if(autonMode == 0){ //If auton is selected...
      lcdPrint(LCDSCREEN, 2, "Battery: %f", batteryVoltage); //Display battery percentage
    } else { //If no auton is selected...
      lcdPrint(LCDSCREEN, 2, "Auton Selected!"); //Confirmation for selected auton
    }

    if((autonMode !=0) && (lcdReadButtons(LCDSCREEN) == 2)){ //If auton is selected and button is pressed
      autonMode = 0; //Un-select autonomous
      delay(300);
    }

    if((autonMode = 0) && lcdReadButtons(LCDSCREEN) == 1){ //If left button is pressed and auton is not selected
      lcdPageNumber -= 1; //Decrease page number
      delay(300);
    }
    if((autonMode = 0) && lcdReadButtons(LCDSCREEN) == 4){ //If right button is pressed and auton is not selected
      lcdPageNumber += 1; //Increase page number
      delay(300);
    }
    if (lcdPageNumber > maxPages){ //Cycle to beginning from end
      lcdPageNumber = mainPage;
    }
    if(lcdPageNumber < mainPage){ //Cycle to end from beginning
      lcdPageNumber = maxPages;
    }

    if (lcdReadButtons(LCDSCREEN) == 2){ //If middle button is pressed, select corresponding autonomous routine
      autonMode = lcdPageNumber;
      delay(300);
    }

    switch(lcdPageNumber){  //Determines what to display on screen based on which page number
      case 1 :
      lcdPrint(LCDSCREEN, 1, "First Routine");
      case 2 :
      lcdPrint(LCDSCREEN, 1, "Second Routine");
      case 3 :
      lcdPrint(LCDSCREEN, 1, "Third Routine");
      case 4 :
      lcdPrint(LCDSCREEN, 1, "Fourth Routine");
      case 5 :
      lcdPrint(LCDSCREEN, 1, "Fifth Routine");
      case 6 :
      lcdPrint(LCDSCREEN, 1, "Sixth Routine");
      case 7 :
      lcdPrint(LCDSCREEN, 1, "Seventh Routine");
      case 8 :
      lcdPrint(LCDSCREEN, 1, "Eighth Routine");
    }
  }
}

void initialize() {

  analogCalibrate(MAINLIFTPOT); //Initializes analog sensors
  analogCalibrate(SHIFTERPOT);
  analogCalibrate(GYROSCOPE);

  encoderInit(LEFTDRIVETOP, LEFTDRIVEBOTTOM, false);  //Initializes quad encoders
  encoderInit(RIGHTDRIVETOP, RIGHTDRIVEBOTTOM, false);

  pinMode(MOGOLIMITSWITCH, INPUT); //Initializes limit switch

  lcdInit(LCDSCREEN);
  lcdClear(LCDSCREEN);
  lcdSetBacklight(LCDSCREEN, true);

  autonSelect();

}
