//////////////////////
/// NORMAL FOSSET  /// - Water in... 
//////////////////////
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
  _NfossetSignalOn() = 1;
}
void nfossetClose(){
  digitalWrite(NFOSSET_PIN, LOW);
  _NfossetSignalOn() = 0;
}

void nfossetManualControl(FossetManualRequestValue value){
  switch (value)
  {
    case FossetManualRequestValue::Close:{
      _NFossetManualRequest = value;
      nfossetClose();
      break;
    }
    case FossetManualRequestValue::Open:{
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

int _NfossetSignalOn();

bool nfossetIsSignalOn(){
  if(_NfossetSignalOn() == 1){
    return true;
  }
  else
    return false;
}

StatusResult nfossetStatus_Short(){
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

bool nfossetSuposeToRun(){
  return ((_NFossetManualRequest == FossetManualRequestValue::FossetAutomatic && nfossetCanRunVar) || _NFossetManualRequest == FossetManualRequestValue::Open);//complete by the enum     //c//
}
String nfossetStatus_Long(){
  bool nfossetWaterFlowing = nfossetIsWaterFlowing();// Normal fosset
  bool nfossetCurrentOn = nfossetIsCurrentOn();
  bool nfossetSignalOn = nfossetIsSignalOn();
  bool nfossetCanRunVar = nfossetCanRun();
  bool nfossetSuposeToRun = nfossetSuposeToRun();

  StatusResult flowingResult = StatusResult("water flowing", "no water", Error);
  
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

bool nfossetIsWaterFlowing(){
  //TODO
}

////////////////////////////////////////////
/// NORMAL FOSST SOLONOID CURRENT TEST ///
////////////////////////////////////////////

void nfossetCurrentInit(){
  pinMode(NFOSSET_CURRENT_TEST_PIN, INPUT);
}

void nfossetCurrentTest(){
  //Serial.print("nfosset current:");
  //Serial.println(nfossetCurrentRead());
}

bool nfossetIsCurrentOn(){
  int c = analogRead(NFOSSET_CURRENT_TEST_PIN);
  return c > 0; //Cheeks if the current is higer than 0. If its higer than 0 its true if not false.
}
