/////////////////
/// TURBIDITY ///
/////////////////

void turbidityTest(){
  int value = turbidityReadPercent();
  Serial.println(value);
}

void turbidityInit()
{
    pinMode(TURBIDITY_PIN, INPUT);
}

int turbidityReadPercent(){
  int val = analogRead(TURBIDITY_PIN);  //Return status in %
  return map(val, 0, 821, 0, 100);
}

bool turbidityIsWorking(){
  int val = analogRead(TURBIDITY_PIN);
  if(val != 0){
    return true;
  }
  else{
    return false;
  }
}

void turbidityCheckStatus(){ //Finish the turbidity Check Status function today.
  int turbidityPercent = turbidityReadPercent();
  bool turbidityIsWorking = turbidityIsWorking();
  
  
}






String turbidityStatus_Short(){ //TURBIDITY STATUS FOR THE HOME SCREEN (AR&AP)
  bool IsWorking = tubidityIsWorking(); //change it to the minimum and the maximum values, with % . 
  int turbidityValue = turbidityReadPercent();
  
  if(IsWorking == true)
    return String(turbidityValue) + "%";
  else
    return "Error";
}

String turbidityStatusLong(){ //The shortest long status for now...  
  bool isWorking = tubidityIsWorking();
  int turbidityValue = turbidityReadPercent();
  
  if(isWorking == true)
    return "Active" + turbidityValue;
  else
    return "Error, Cheek the sensor";
}
