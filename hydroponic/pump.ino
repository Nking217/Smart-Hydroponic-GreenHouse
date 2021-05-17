//////////////
///  PUMP  /// //Liquids to the system...
//////////////
void pumpTest(){
  pumpStart();
  delay(500);
  pumpStop();
  delay(500);
}
void pumpInit(){
  pinMode(PUMP_PIN, OUTPUT);
}

void pumpStart(){
  digitalWrite(PUMP_PIN, HIGH);
  _IsPumpRunning = 1;
}
void pumpStop(){
  digitalWrite(PUMP_PIN, LOW);
  _IsPumpRunning = 0;
}

void pumpManualControl(PumpManualRequestValue value){
  switch (value)
  {
    case PumpManualRequestValue::Start:{
      int h = waterHightRead();
      if(h > MINIMUM_WATER_FOR_PUMP){
        _PumpManualRequest = value;
        pumpStart(); //Pump can run
      }
      break;
    }
    case PumpManualRequestValue::Stop:{
      _PumpManualRequest = value;
      pumpStop();
      break;
    }
    case PumpManualRequestValue::PumpAutomatic:{
      _PumpManualRequest = value;
      break;
    }
  }
}

bool pumpCanRun(){
  int h = waterHightRead();
  if(h > MINIMUM_WATER_FOR_PUMP) //If the water levle is not the maximum the fosset will be open.
    return true;   
  else
    return false;
}

bool pumpIsSignalOn(){
  if(_IsPumpRunning == 1){
    return true;
  }
  else{
    return false;
  }
}
/*
StatusResult pumpStatus_Short(){
  bool isRunning = pumpIsRunning();
  bool isWaterFlowing = pumpIsWaterFlowing();
  bool canRun = pumpCanRun();

  switch (_PumpManualRequest){
    case PumpManualRequestValue::PumpAutomatic:
    {
      if(isWaterFlowing == true)
      {
        if(canRun == true)
          return StatusResult ("On", STATUS_OK);
        else
          return StatusResult ("Error", STATUS_ERROR);
      }
      else 
      {
        if(isRunning)
          return StatusResult ("Error", STATUS_ERROR);
        else 
          return StatusResult ("Off", STATUS_OK);
      }
      break;
    } 

    case PumpManualRequestValue::Start:
    {
      if(isWaterFlowing == true){
        if(canRun)
          return StatusResult ("On", STATUS_OK);
        else
          return StatusResult ("Error", STATUS_ERROR);
      }
      else{ 
        return StatusResult ("Error", STATUS_ERROR);
      }
      break;
    }
    case PumpManualRequestValue::Stop:{
      if(isWaterFlowing == false){
        if(!isRunning)
          return StatusResult ("Off", STATUS_OK);
        else
          return StatusResult ("Error", STATUS_ERROR);
      }
      else{
        return StatusResult ("Error", STATUS_ERROR);
      }
      break;
    }
    
  }
}
*/

bool pumpSuposeToRun(){
  return((_PumpManualRequest == PumpManualRequestValue::PumpAutomatic && pumpCanRun()) || _PumpManualRequest == PumpManualRequestValue::Start);
}
/*
void pumpCheeckStatus(){
  bool pumpWaterFlowing = pumpIsWaterFlowing();
  bool pumpCurrentOn = pumpIsCurrentOn();
  bool pumpSignalOn = pumpIsSignalOn();
  bool pumpSuposeToRunVar = pumpSuposeToRun();

  int h = waterHightRead();

  
}
*/


///////////////////////////
/// PUMP CURRENT SENSOR ///
///////////////////////////

void pumpCurrentInit(){
  pinMode(PUMP_CURRENT_TEST_PIN, INPUT);
 // pinMode( //no pin yet
}

void pumpCurrentTest(){
  //Serial.print("Pump current:");
  //Serial.println(pumpCurrentRead());
}

bool pumpIsCurrentOn(){
  int c = analogRead(PUMP_CURRENT_TEST_PIN);
  return c > 0; //Cheeks if the current is higer than 0. If its higer than 0 its true if not false.
}

////////////////////////
/// PUMP FLOW SENSOR ///
////////////////////////

void pumpFlowInit(){
  pinMode(PUMP_FLOW_TEST_PIN, INPUT);
}

void pumpFlowTest(){
  //TODO
}

bool pumpIsWaterFlowing(){
  //TODO
}
