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
    case PumpManualRequestValue::PumpStart:{
      int h = waterHightRead();
      if(h > MINIMUM_WATER_FOR_PUMP){
        _PumpManualRequest = value;
        pumpStart(); //Pump can run
      }
      break;
    }
    case PumpManualRequestValue::PumpStop:{
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

bool pumpSuposeToRun(){
  return((_PumpManualRequest == PumpManualRequestValue::PumpAutomatic && pumpCanRun()) || _PumpManualRequest == PumpManualRequestValue::PumpStart);
}

void pumpCheckStatus(){
  bool pumpWaterFlowing = pumpIsWaterFlowing();
  bool pumpCurrentOn = pumpIsCurrentOn();
  bool pumpSignalOn = pumpIsSignalOn();
  bool pumpSuposeToRunVar = pumpSuposeToRun();

  int h = waterHightRead();

  //Check Signal On
  if(_PumpManualRequest == PumpManualRequestValue::PumpAutomatic){
    if(h <= MINIMUM_WATER_FOR_PUMP){
      if(pumpSignalOn){
        _PumpStatus.SignalOn = StatusResult("Error, Signal on (automatic) when water levle is too low", STATUS_ERROR, PRIORITY_HIGH);
        _PumpStatus.Short = StatusResult("Error", STATUS_ERROR, PRIORITY_HIGH);
      }
      else{
        _PumpStatus.SignalOn = StatusResult("Warning, Signal off (automatic) Water levle is low", STATUS_WARNING, PRIORITY_HIGH);
        _PumpStatus.Short = StatusResult("Warning", STATUS_WARNING, PRIORITY_HIGH);
      }
    }
    else{
      if(pumpSignalOn){
        _PumpStatus.SignalOn = StatusResult("On", STATUS_OK, PRIORITY_LOW);
        _PumpStatus.Short = StatusResult("On", STATUS_OK, PRIORITY_LOW);
      }
      else{
        _PumpStatus.SignalOn = StatusResult("Off", STATUS_OK, PRIORITY_LOW);
        _PumpStatus.Short = StatusResult("Off", STATUS_OK, PRIORITY_LOW);
      }
    }
  }
  else if(_PumpManualRequest == PumpManualRequestValue::PumpStart){
    if(h <= MINIMUM_WATER_FOR_PUMP){
      if(pumpSignalOn){
        _PumpStatus.SignalOn = StatusResult("Error, Signal on (manual) when water levle is too low", STATUS_ERROR, PRIORITY_HIGH);
        _PumpStatus.Short = StatusResult("Error", STATUS_ERROR, PRIORITY_HIGH);
      }
      else{
        _PumpStatus.SignalOn = StatusResult("Warning, Signal off (manual) Water levle is low", STATUS_WARNING, PRIORITY_HIGH);
        _PumpStatus.Short = StatusResult("Warning", STATUS_WARNING, PRIORITY_HIGH);
      }
    }
    else{
      if(pumpSignalOn){
        _PumpStatus.SignalOn = StatusResult("On (manual)", STATUS_OK, PRIORITY_LOW);
        _PumpStatus.Short = StatusResult("On", STATUS_OK, PRIORITY_LOW);
      }
      else{
        _PumpStatus.SignalOn = StatusResult("Error, Signal off (manual) while manual request is on ", STATUS_ERROR, PRIORITY_HIGH);
        _PumpStatus.Short = StatusResult("Error", STATUS_ERROR, PRIORITY_HIGH);
      }
    }
  }
  else{ //Manual stop
    if(pumpSignalOn){
        _PumpStatus.SignalOn = StatusResult("Error, Signal on while manual is off", STATUS_ERROR, PRIORITY_HIGH);
        _PumpStatus.Short = StatusResult("Error", STATUS_ERROR, PRIORITY_HIGH);
    }
    else {
      if(h <= MINIMUM_WATER_FOR_PUMP){
        _PumpStatus.SignalOn = StatusResult("Warning, Signal off (manual) Water levle is low", STATUS_WARNING, PRIORITY_HIGH);
        _PumpStatus.Short = StatusResult("Warning", STATUS_WARNING, PRIORITY_HIGH);
      }        
      else {
        _PumpStatus.SignalOn = StatusResult("Off (manual)", STATUS_OK, PRIORITY_LOW);
        _PumpStatus.Short = StatusResult("Off", STATUS_OK, PRIORITY_LOW);
      }
    }
  }
  //Check can run
  if(h <= MINIMUM_WATER_FOR_PUMP){
    _PumpStatus.CanRun = StatusResult("Error: Not enough water for pump", STATUS_ERROR, PRIORITY_HIGH);
    _PumpStatus.Short = StatusResult("Error", STATUS_ERROR, PRIORITY_HIGH);
  }
  else{
    _PumpStatus.CanRun = StatusResult("Ok", STATUS_OK, PRIORITY_LOW);
  }
  //Check manual request
  if(_PumpManualRequest == PumpManualRequestValue::PumpAutomatic){  //((_PumpManualRequest == PumpManualRequestValue::PumpAutomatic && pumpCanRun()) || _PumpManualRequest == PumpManualRequestValue::PumpStart);
    if(h <= MINIMUM_WATER_FOR_PUMP){
       _PumpStatus.SuposeToRun = StatusResult("No (automatic), Not enough water", STATUS_WARNING, PRIORITY_HIGH);
    }
    else{
      _PumpStatus.SuposeToRun = StatusResult("Yes (automatic)", STATUS_OK, PRIORITY_LOW);
    }
  }
  else if(_PumpManualRequest == PumpManualRequestValue::PumpStart){
    if(h <= MINIMUM_WATER_FOR_PUMP){
      _PumpStatus.SuposeToRun = StatusResult("Error, Yes (manual), Not enough water", STATUS_ERROR, PRIORITY_HIGH);
      _PumpStatus.Short = StatusResult("Error", STATUS_ERROR, PRIORITY_HIGH);
    }
    else{
      _PumpStatus.SuposeToRun = StatusResult("Yes (manual)", STATUS_OK, PRIORITY_LOW); 
    }
  }
  else{ //Manual stop
    if(h <= MINIMUM_WATER_FOR_PUMP){
       _PumpStatus.SuposeToRun = StatusResult("No (manual), Not enough water", STATUS_WARNING, PRIORITY_HIGH);
    }
    else{
      _PumpStatus.SuposeToRun = StatusResult("Yes (manual)", STATUS_OK, PRIORITY_LOW);
    }
  }
  //Check current on
  if(pumpSignalOn){
    if(!pumpCurrentOn){
      _PumpStatus.CurrentOn = StatusResult("Error, No current when signal is on", STATUS_ERROR, PRIORITY_HIGH);
      _PumpStatus.Short = StatusResult("Error", STATUS_ERROR, PRIORITY_HIGH);
    }
    else{
      _PumpStatus.CurrentOn = StatusResult("On", STATUS_OK, PRIORITY_LOW);
    }
  }
  else{
    if(!pumpCurrentOn){
      _PumpStatus.CurrentOn = StatusResult("Off", STATUS_OK, PRIORITY_LOW);
    }
    else{
      _PumpStatus.CurrentOn = StatusResult("Error, Current while signal off", STATUS_ERROR, PRIORITY_HIGH);
      _PumpStatus.Short = StatusResult("Error", STATUS_ERROR, PRIORITY_HIGH);
    }
  }
  //Check Water Flowing
  if(pumpSignalOn){
    if(!pumpWaterFlowing){
      _PumpStatus.CurrentOn = StatusResult("Error, Water not flowing while signal is on", STATUS_ERROR, PRIORITY_HIGH);
      _PumpStatus.Short = StatusResult("Error", STATUS_ERROR, PRIORITY_HIGH);
    }
    else{
      _PumpStatus.CurrentOn = StatusResult("On", STATUS_OK, PRIORITY_LOW);
    }
  }
  else{
    if(!pumpWaterFlowing){
      _PumpStatus.CurrentOn = StatusResult("Off", STATUS_OK, PRIORITY_LOW);
    }
    else{
      _PumpStatus.CurrentOn = StatusResult("Error, Water flowing while signal is off", STATUS_ERROR, PRIORITY_HIGH);
      _PumpStatus.Short = StatusResult("Error", STATUS_ERROR, PRIORITY_HIGH);
    }
  }
}

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
  //Not ready for now
}

bool pumpIsWaterFlowing(){ //Not completed yet (for now its on while the current and the signal is on)
  if(pumpIsCurrentOn() && pumpIsSignalOn()){
    return true;
  }
  else{
    return false;
  }
}
