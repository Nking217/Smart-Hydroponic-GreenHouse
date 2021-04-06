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

int turbidityReadPercent()
{
  int val = analogRead(TURBIDITY_PIN);  //Return status in %
  return map(val, 0, 1024, 0, 100);
//  return val * (5.0 / 1024.0);
}

bool tubidityIsWorking(){ //fix this when you can run tests
  return true;
}



String turbidityStatus_Short(){ //TURBIDITY STATUS FOR THE HOME SCREEN (AR&AP)
  bool IsWorking = tubidityIsWorking(); //change it to the minimum and the maximum values, with % . 
  int turbidityValue = turbidityReadPercent();
  
  if(IsWorking == true)
    return String(turbidityValue) + "%";
  else
    return "Error";
}

String turbidityStatusLong(){ //The shortest long status for now...  ////TEST////
  bool isWorking = tubidityIsWorking();
  int turbidityValue = turbidityReadPercent();
  
  if(isWorking == true)
    return "Active" + turbidityValue;
  else
    return "Error, Cheek the sensor";
}
