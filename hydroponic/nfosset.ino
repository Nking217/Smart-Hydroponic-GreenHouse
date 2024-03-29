//////////////////////
/// NORMAL FOSSET  /// - Water in... 
//////////////////////
/*
class nfossetStatus{
  public:
    StatusResult WaterFlowing;
    StatusResult CurrentOn;
    StatusResult SignalOn;
    StatusResult CanRun;
    StatusResult SuposeToRun;

    StatusResult Short;

    String getShortString(){
      
    }

};

nfossetStatus _NfossetStatus;
*/
void nfossetTest(){
  nfossetOpen();
  delay(500);
  nfossetClose();
  delay(500);
}
void nfossetInit(){
  pinMode(NFOSSET_PIN, OUTPUT);
}

void nfossetOpen(){
  digitalWrite(NFOSSET_PIN, HIGH);
  _NfossetSignalOn = 1;
}
void nfossetClose(){
  digitalWrite(NFOSSET_PIN, LOW);
  _NfossetSignalOn = 0;
}

void nfossetManualControl(FossetManualRequestValue value){
  switch (value)
  {
    case FossetManualRequestValue::FossetClose:{
      _NFossetManualRequest = value;
      nfossetClose();
      break;
    }
    case FossetManualRequestValue::FossetOpen:{
      if(nfossetCanRun()){ //If the water levle is not the maximum the nfosset will be open.
        _NFossetManualRequest = value;
        nfossetOpen();
      }
      break;
    }
    case FossetManualRequestValue::FossetAutomatic:{
      _NFossetManualRequest = value;
      break;
    }
  }
}
bool nfossetCanRun(){
  int h = waterHightRead();
  if(h < MAXIMUM_WATER_IN_TANK) //If the water levle is not the maximum the fosset will be open.
    return true;
  else
    return false;
}

//int _NfossetSignalOn;

bool nfossetIsSignalOn(){
  if(_NfossetSignalOn == 1){
    return true;
  }
  else
    return false;
}
/*
StatusResult nfossetStatus_Short(){ //// Short Status - old code ////
  bool nfossetFlowing = nfossetIsWaterFlowing();// Normal fosset
  bool nfossetCanRunVar = nfossetCanRun();
  
  switch (_NFossetManualRequest) {
    case FossetManualRequestValue::FossetAutomatic:
    {
      if(nfossetFlowing == true)
      {
         if(nfossetCanRunVar == true)
            return StatusResult ("On", STATUS_OK);
         else
            return StatusResult ("Error", STATUS_ERROR);
      }
      else
      {
         return StatusResult ("Off", STATUS_OK);
      }
      break;
    }
    case FossetManualRequestValue::Open:
    {
      if(nfossetFlowing == true)
        return StatusResult ("On", STATUS_OK);
      else
        return StatusResult ("Error", STATUS_ERROR);
      break;
    }
    case FossetManualRequestValue::Close:
      if(nfossetFlowing == false)
        return StatusResult ("Off", STATUS_OK);
      else
        return StatusResult ("Error", STATUS_ERROR);
      break;
    default:  
      return StatusResult ("Unknown", STATUS_WARNING);
      
  }
}
*/
bool nfossetCanRunVar = nfossetCanRun();

bool nfossetSuposeToRun(){
  return ((_NFossetManualRequest == FossetManualRequestValue::FossetAutomatic && nfossetCanRunVar) || _NFossetManualRequest == FossetManualRequestValue::FossetOpen);//complete by the enum     //c//
}


void nfossetCheckStatus(){
  bool nfossetWaterFlowing = nfossetIsWaterFlowing();
  bool nfossetCurrentOn = nfossetIsCurrentOn();
  bool nfossetSignalOn = nfossetIsSignalOn();
  bool nfossetSuposeToRunVar = nfossetSuposeToRun();

  int h = waterHightRead();

  if(h >= MAXIMUM_WATER_IN_TANK){ //If the water levle is the maximum levle for the tank, the fosset will be closed.
    if(nfossetSignalOn){
      _NfossetStatus.SignalOn = StatusResult("Error: Fosset signal pin on while water levle too high", STATUS_ERROR, PRIORITY_HIGH);
      _NfossetStatus.Short = StatusResult("Error", STATUS_ERROR, PRIORITY_HIGH);
    }
    else{
      _NfossetStatus.SignalOn = StatusResult("Off: Fosset signal off", STATUS_OK, PRIORITY_LOW);
      _NfossetStatus.Short = StatusResult("Off", STATUS_OK, PRIORITY_LOW);
    }
  }
  else{
    if(nfossetSuposeToRun){
      if(!nfossetSignalOn){
        if(h > MINIMUM_WATER_FOR_PUMP){
          _NfossetStatus.SignalOn = StatusResult("Warning: Fosset signal pin off while water levle is low", STATUS_WARNING, PRIORITY_MEDIUM);
          _NfossetStatus.Short = StatusResult("Warning", STATUS_WARNING, PRIORITY_MEDIUM);
        }
        else{
          _NfossetStatus.SignalOn = StatusResult("Error: Fosset signal pin off while water levle is under pump minimum", STATUS_ERROR, PRIORITY_HIGH);
          _NfossetStatus.Short = StatusResult("Error", STATUS_ERROR, PRIORITY_HIGH);
        }
      }
      else{
        _NfossetStatus.SignalOn = StatusResult("On: Fosset signal pin on", STATUS_OK, PRIORITY_LOW);
        _NfossetStatus.Short = StatusResult("On", STATUS_OK, PRIORITY_LOW);
      }
    }
  }
    
    
  if(nfossetSignalOn){
     if(!nfossetWaterFlowing){
        if(h > MINIMUM_WATER_FOR_PUMP){
          _NfossetStatus.WaterFlowing = StatusResult("Warning: Water not flowing while signal pin is on", STATUS_WARNING, PRIORITY_MEDIUM);
          _NfossetStatus.Short = StatusResult("Warning", STATUS_WARNING, PRIORITY_MEDIUM);
        }
        else{
          _NfossetStatus.WaterFlowing = StatusResult("Error: Water not flowing while water levle is under pump minimum", STATUS_ERROR, PRIORITY_HIGH);
          _NfossetStatus.Short = StatusResult("Error", STATUS_ERROR, PRIORITY_HIGH);
        }
     }
     else{
        _NfossetStatus.WaterFlowing = StatusResult("On: Water flowing", STATUS_OK, PRIORITY_LOW);
        if(_NfossetStatus.Short.Status == STATUS_OK){
          _NfossetStatus.Short = StatusResult("On", STATUS_OK, PRIORITY_LOW);
        }
     }
     if(!nfossetCurrentOn){
        if(h > MINIMUM_WATER_FOR_PUMP){
          _NfossetStatus.CurrentOn = StatusResult("Warning: No current while signal pin is on", STATUS_WARNING, PRIORITY_MEDIUM);
          if(_NfossetStatus.Short.Status != STATUS_ERROR){
            _NfossetStatus.Short = StatusResult("Warning", STATUS_WARNING, PRIORITY_MEDIUM);
          }
        }
        else{
          _NfossetStatus.CurrentOn = StatusResult("Error: No current while water levle is under pump minimum", STATUS_ERROR, PRIORITY_HIGH);
          _NfossetStatus.Short = StatusResult("Error", STATUS_ERROR, PRIORITY_HIGH);
        }
     }
     else{
        _NfossetStatus.CurrentOn = StatusResult("Current on", STATUS_OK, PRIORITY_LOW);
        if(_NfossetStatus.Short.Status == STATUS_OK){
          _NfossetStatus.Short = StatusResult("On", STATUS_OK, PRIORITY_LOW);
        }
     }
  }
  else{
     if(nfossetWaterFlowing){
        _NfossetStatus.WaterFlowing = StatusResult("Error: Water flowing while signal pin is off", STATUS_ERROR, PRIORITY_HIGH);
        _NfossetStatus.Short = StatusResult("Error", STATUS_ERROR, PRIORITY_HIGH);
     }
     else{
        _NfossetStatus.WaterFlowing = StatusResult("No water flowing", STATUS_OK, PRIORITY_LOW);
        if(_NfossetStatus.Short.Status == STATUS_OK){
          _NfossetStatus.Short = StatusResult("Off", STATUS_OK, PRIORITY_LOW);
        }
     }
     if(nfossetCurrentOn){
        _NfossetStatus.CurrentOn = StatusResult("Error: Fosset has current while signal pin is off", STATUS_ERROR, PRIORITY_HIGH);
        _NfossetStatus.Short = StatusResult("Error", STATUS_ERROR, PRIORITY_HIGH);
     }
     else{
        _NfossetStatus.CurrentOn = StatusResult("No current", STATUS_OK, PRIORITY_LOW);
        if(_NfossetStatus.Short.Status == STATUS_OK){
          _NfossetStatus.Short = StatusResult("Off", STATUS_OK, PRIORITY_LOW);
        }
     }
  }
}


/////////////////////////////////
/// NORMAL FOSSET FLOW SENSOR ///
/////////////////////////////////

void nfossetFlowInit(){
  pinMode(NFOSSET_FLOW_TEST_PIN, INPUT);
}

void nfossetFlowTest(){
  //TODO
}

bool nfossetIsWaterFlowing(){ //Not completed yet (for now its on while the current and the signal is on)
  if(nfossetIsCurrentOn() && nfossetIsSignalOn()){
    return true;
  }
  else{
    return false;
  }
}

/////////////////////////////////
/// NORMAL FOSST CURRENT TEST ///
/////////////////////////////////

void nfossetCurrentInit(){
  pinMode(NFOSSET_CURRENT_TEST_PIN, INPUT);
}

void nfossetCurrentTest(){
  Serial.print("nfosset current:");
  Serial.println(analogRead(NFOSSET_CURRENT_TEST_PIN));
}

bool nfossetIsCurrentOn(){
  int c = analogRead(NFOSSET_CURRENT_TEST_PIN);
  return c > 0; //Cheeks if the current is higer than 0. If its higer than 0 its true if not false.
}
