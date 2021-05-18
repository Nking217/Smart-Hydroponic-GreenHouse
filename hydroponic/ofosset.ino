///////////////////////
/// OSMOTIC FOSSET  /// - Water in... 
///////////////////////
int _OfossetSignalOn;
/*
class ofossetStatus{
  public:
    StatusResult WaterFlowing;
    StatusResult CurrentOn;
    StatusResult SignalOn;
    StatusResult CanRun;
    StatusResult SuposeToRun;

    StatusResult Short;    
};

ofossetStatus _OfossetStatus;
*/
void ofossetTest(){
  ofossetOpen();
  delay(500);
  ofossetClose();
  delay(500);
}
void ofossetInit(){
  pinMode(OFOSSET_PIN, OUTPUT); 
}

void ofossetOpen(){
  digitalWrite(OFOSSET_PIN, HIGH);
  _OfossetSignalOn = 1;
}
void ofossetClose(){
  digitalWrite(OFOSSET_PIN, LOW);
  _OfossetSignalOn = 0;
}

void ofossetManualControl(FossetManualRequestValue value){
  switch (value)
  {
    case FossetManualRequestValue::FossetClose:{
      _OFossetManualRequest = value;
      ofossetClose();
      break;
    }
    case FossetManualRequestValue::FossetOpen:{
      if(ofossetCanRun()){ //If the water levle is not the maximum the fosset will be open.
        _OFossetManualRequest = value;
        ofossetOpen();
      }
      break;
    }
    case FossetManualRequestValue::FossetAutomatic:{
      _OFossetManualRequest = value;
      break;
    }
  }
}

bool ofossetCanRun(){
  int h = waterHightRead();
  if(h < MAXIMUM_WATER_IN_TANK) //If the water levle is not the maximum the fosset will be open.
    return true;    
  else
    return false;
}


bool ofossetIsSignalOn(){
  if(_OfossetSignalOn == 1){
    return true;
  }
  else
    return false;
}
/*
StatusResult ofossetStatus_Short(){
  bool ofossetFlowing = ofossetIsWaterFlowing();// osmotic fosset
  bool ofossetCanRunVar = ofossetCanRun();
  
  switch (_OFossetManualRequest) {
    case FossetManualRequestValue::FossetAutomatic:
    {
      if(ofossetFlowing == true)
      {
         if(ofossetCanRunVar == true)
            return StatusResult ("On", STATUS_OK);
         else
            return StatusResult ("Error", STATUS_ERROR);
      }
      else
      {
         return StatusResult ("Off", STATUS_OK); //a
      }
      break;
    }
    case FossetManualRequestValue::Open:
    {
      if(ofossetFlowing == true)
        return StatusResult ("On", STATUS_OK);
      else
        return StatusResult ("Error", STATUS_ERROR);
      break;
    }
    case FossetManualRequestValue::Close:
      if(ofossetFlowing == false)
        return StatusResult ("Off", STATUS_OK);
      else
        return StatusResult ("Error", STATUS_ERROR);
      break;
    default:  
      return StatusResult ("Unknown", STATUS_WARNING);
      
  }
}
*/

//bool ofossetCanRunVar = ofossetCanRun();

bool ofossetSuposeToRun(){
  return ((_OFossetManualRequest == FossetManualRequestValue::FossetAutomatic && ofossetCanRun()) || _OFossetManualRequest == FossetManualRequestValue::FossetOpen);  //complete by the enum
}


void ofossetCheckStatus(){
  bool ofossetWaterFlowing = ofossetIsWaterFlowing();
  bool ofossetCurrentOn = ofossetIsCurrentOn();
  bool ofossetSignalOn = ofossetIsSignalOn();
  bool ofossetSuposeToRunVar = ofossetSuposeToRun();

  int h = waterHightRead();

  if(h >= MAXIMUM_WATER_IN_TANK){ //If the water levle is the maximum levle for the tank, the fosset will be closed.
    if(ofossetSignalOn){
      _OfossetStatus.SignalOn = StatusResult("Error: Fosset signal pin on while water levle too high", STATUS_ERROR, PRIORITY_HIGH);
      _OfossetStatus.Short = StatusResult("Error", STATUS_ERROR, PRIORITY_HIGH);
    }
    else{
      _OfossetStatus.SignalOn = StatusResult("Off: Fosset signal off", STATUS_OK, PRIORITY_LOW);
      _OfossetStatus.Short = StatusResult("Off", STATUS_OK, PRIORITY_LOW);
    }
  }
  else{
    if(ofossetSuposeToRun){
      if(!ofossetSignalOn){
        if(h > MINIMUM_WATER_FOR_PUMP){
          _OfossetStatus.SignalOn = StatusResult("Warning: Fosset signal pin off while water levle is low", STATUS_WARNING, PRIORITY_MEDIUM);
          _OfossetStatus.Short = StatusResult("Warning", STATUS_WARNING, PRIORITY_MEDIUM);
        }
        else{
          _OfossetStatus.SignalOn = StatusResult("Error: Fosset signal pin off while water levle is under pump minimum", STATUS_ERROR, PRIORITY_HIGH);
          _OfossetStatus.Short = StatusResult("Error", STATUS_ERROR, PRIORITY_HIGH);
        }
      }
      else{
        _OfossetStatus.SignalOn = StatusResult("On: Fosset signal pin on", STATUS_OK, PRIORITY_LOW);
        _OfossetStatus.Short = StatusResult("On", STATUS_OK, PRIORITY_LOW);
      }
    }
  }
    
    
  if(ofossetSignalOn){
     if(!ofossetWaterFlowing){
        if(h > MINIMUM_WATER_FOR_PUMP){
          _OfossetStatus.WaterFlowing = StatusResult("Warning: Water not flowing while signal pin is on", STATUS_WARNING, PRIORITY_MEDIUM);
          _OfossetStatus.Short = StatusResult("Warning", STATUS_WARNING, PRIORITY_MEDIUM);
        }
        else{
          _OfossetStatus.WaterFlowing = StatusResult("Error: Water not flowing while water levle is under pump minimum", STATUS_ERROR, PRIORITY_HIGH);
          _OfossetStatus.Short = StatusResult("Error", STATUS_ERROR, PRIORITY_HIGH);
        }
     }
     else{
        _OfossetStatus.WaterFlowing = StatusResult("On: Water flowing", STATUS_OK, PRIORITY_LOW);
        if(_OfossetStatus.Short.Status == STATUS_OK){
          _OfossetStatus.Short = StatusResult("On", STATUS_OK, PRIORITY_LOW);
        }
     }
     if(!ofossetCurrentOn){
        if(h > MINIMUM_WATER_FOR_PUMP){
          _OfossetStatus.CurrentOn = StatusResult("Warning: No current while signal pin is on", STATUS_WARNING, PRIORITY_MEDIUM);
          if(_OfossetStatus.Short.Status != STATUS_ERROR){
            _OfossetStatus.Short = StatusResult("Warning", STATUS_WARNING, PRIORITY_MEDIUM);
          }
        }
        else{
          _OfossetStatus.CurrentOn = StatusResult("Error: No current while water levle is under pump minimum", STATUS_ERROR, PRIORITY_HIGH);
          _OfossetStatus.Short = StatusResult("Error", STATUS_ERROR, PRIORITY_HIGH);
        }
     }
     else{
        _OfossetStatus.CurrentOn = StatusResult("Current on", STATUS_OK, PRIORITY_LOW);
        if(_OfossetStatus.Short.Status == STATUS_OK){
          _OfossetStatus.Short = StatusResult("On", STATUS_OK, PRIORITY_LOW);
        }
     }
  }
  else{
     if(ofossetWaterFlowing){
        _OfossetStatus.WaterFlowing = StatusResult("Error: Water flowing while signal pin is off", STATUS_ERROR, PRIORITY_HIGH);
        _OfossetStatus.Short = StatusResult("Error", STATUS_ERROR, PRIORITY_HIGH);
     }
     else{
        _OfossetStatus.WaterFlowing = StatusResult("No water flowing", STATUS_OK, PRIORITY_LOW);
        if(_OfossetStatus.Short.Status == STATUS_OK){
          _OfossetStatus.Short = StatusResult("Off", STATUS_OK, PRIORITY_LOW);
        }
     }
     if(ofossetCurrentOn){
        _OfossetStatus.CurrentOn = StatusResult("Error: Fosset has current while signal pin is off", STATUS_ERROR, PRIORITY_HIGH);
        _OfossetStatus.Short = StatusResult("Error", STATUS_ERROR, PRIORITY_HIGH);
     }
     else{
        _OfossetStatus.CurrentOn = StatusResult("No current", STATUS_OK, PRIORITY_LOW);
        if(_OfossetStatus.Short.Status == STATUS_OK){
          _OfossetStatus.Short = StatusResult("Off", STATUS_OK, PRIORITY_LOW);
        }
     }
  }
}


//////////////////////////////////
/// OSMOTIC FOSSET FLOW SENSOR ///
//////////////////////////////////

void ofossetFlowInit(){
  pinMode(OFOSSET_FLOW_TEST_PIN, INPUT);
}

void ofossetFlowTest(){
  //TODO
}

bool ofossetIsWaterFlowing(){
  //TODO
}
///////////////////////////////////
/// OSMOTIC FOSSET CURRENT TEST ///
///////////////////////////////////

void ofossetCurrentInit(){
  pinMode(OFOSSET_CURRENT_TEST_PIN, INPUT);
}

void ofossetCurrentTest(){
  //Serial.print("ofosset current:");
  //Serial.println(ofossetCurrentRead());
}

bool ofossetIsCurrentOn(){
  int c = analogRead(OFOSSET_CURRENT_TEST_PIN);
  return c > 0; //Cheeks if the current is higer than 0. If its higer than 0 its true if not false.
}
