/////////////////////
///  WATER HIGHT  ///
/////////////////////

void waterHightTest(){
  //Sensor test
  Serial.print("Water low sensor:"); 
  Serial.println(digitalRead(WATER_LOW_SWITCH_PIN)); 
  Serial.print("Water high sensor:"); 
  Serial.println(digitalRead(WATER_HIGH_SWITCH_PIN)); 
  delay(1000);
  //Function test
  //Serial.print("Water hight test:"); 
  //Serial.println(waterHightRead()); 
}

void waterHightInit(){
  pinMode(WATER_HIGH_SWITCH_PIN, INPUT);
  pinMode(WATER_LOW_SWITCH_PIN, INPUT);
}

int waterHightRead(){
  bool waterInLow = !digitalRead(WATER_LOW_SWITCH_PIN); //* off/down = 1,  on/up = 0
  bool waterInHigh = !digitalRead(WATER_HIGH_SWITCH_PIN);  //* true 1 , false 0.
  if(!waterInLow){
    return 0;
  }
  else if(waterInHigh){
    return 100;
  }
  else if (!waterInHigh && waterInLow){
    return 50;
  }
  else {
    return ERROR;
  }
}
