void partnerController(void * parameter) {
	while(1){
		if (joystickGetDigital(2, 7, JOY_UP)){ //Sets to mobile goal mode
			scoringMode = 0;
			stackedCones = 0;
			delay(300);
		} else if (joystickGetDigital(2, 7, JOY_DOWN)){ //Sets to stationary goal mode
			scoringMode = 1;
			delay(300);
		} else if (joystickGetDigital(2, 7, JOY_LEFT)){ //Sets to manual control
			scoringMode = 2;
			delay(300);
		}


		if (joystickGetDigital(2, 8, JOY_UP)){ //Sets to autoloader pickup
			pickupMode = 0;
			delay(300);
		} else if (joystickGetDigital(2, 8, JOY_DOWN)){ //Sets to ground pickup
			pickupMode = 1;
			delay(300);
		}

		if(joystickGetDigital(2, 6, JOY_UP)){  //Increments cone counts
			if(scoringMode == 0){					//depending on scoringMode
			stackedCones += 1;
			}	else if (scoringMode ==1){
				stationaryCones += 1;
			}
			delay(300);
		}
		if(joystickGetDigital(2, 6, JOY_DOWN)){  //Decrements cone counts
			if(scoringMode == 0){					//depending on scoringMode
			stackedCones -= 1;
			}	else if (scoringMode ==1){
				stationaryCones -= 1;
			}
			delay(300);
		}

		if(joystickGetDigital(2, 5, JOY_DOWN)){ //Sets stacked cone number to 5
			if(scoringMode == 0){
			stackedCones = 5;  //Change mogo cones count to 5
			} else if (scoringMode == 1){
				stationaryCones = 5;  //Change statgo cones count to 10
			}
			delay(300);
		}
		if(joystickGetDigital(2, 5, JOY_UP)){ //Sets stacked cone number to 10
			if(scoringMode == 0){
			stackedCones = 10;  //Change mogo cones count to 10
		} else if (scoringMode == 1){
			stationaryCones = 10;  //Change statgo cones count to 10
		}
			delay(300);
		}

		delay(20);
	}
}
