///////////////////////
/// OSMOTIC FOSSET  /// - Water in... 
///////////////////////
/*
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
}
void ofossetClose(){
  digitalWrite(OFOSSET_PIN, LOW);
}

void ofossetManualControl(FossetManualRequestValue value){
  switch (value)
  {
    case FossetManualRequestValue::Close:{
      _OFossetManualRequest = value;
      ofossetClose();
      break;
    }
    case FossetManualRequestValue::Open:{
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

String ofossetStatus_Long(){
  bool ofossetFlowing = ofossetIsWaterFlowing();// Normal fosset
  bool ofossetCurrentOn = ofossetIsRunning();
  bool ofossetCanRunVar = ofossetCanRun();
  bool ofossetSuposeToRun = ((_OFossetManualRequest == FossetManualRequestValue::FossetAutomatic && ofossetCanRunVar) || _OFossetManualRequest == FossetManualRequestValue::Open);//complete by the enum     //c//

  
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

bool ofossetIsRunning(){
  int c = analogRead(OFOSSET_CURRENT_TEST_PIN);
  return c > 0; //Cheeks if the current is higer than 0. If its higer than 0 its true if not false.
}
*/
