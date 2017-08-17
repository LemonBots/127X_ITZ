int8_t deadband(int8_t value){  //Does not return a small motor value
	if(abs(value) < 15){
		return 0;
	} else {
		return value;
	}
}
