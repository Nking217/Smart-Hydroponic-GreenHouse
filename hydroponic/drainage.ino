//////////////////
///  DRAINAGE  ///
//////////////////
bool _DrainageIsOpen;

void drainageTest(){
  drainageOpen();
  delay(500);
  drainageClose();
  delay(500);
}
void drainageInit(){
  pinMode(DRAINAGE_PIN, OUTPUT);
  drainageClose();
}

void drainageOpen(){
  digitalWrite(DRAINAGE_PIN, HIGH);
  _DrainageIsOpen = true;
}
void drainageClose(){
  digitalWrite(DRAINAGE_PIN, LOW);
  _DrainageIsOpen = false;
}

bool drainageIsOpen(){
  return _DrainageIsOpen;
}

void drainageManualControl(DrainageManualRequestValue value){
  switch (value)
  {
    case DrainageManualRequestValue::DrainageClose:{
      _DrainageManualRequest = value;
      drainageClose();
      break;
    }
    case DrainageManualRequestValue::DrainageOpen:{
      _DrainageManualRequest = value;
      drainageOpen();
      break;
    }
    case DrainageManualRequestValue::DrainageAutomatic:{
      _DrainageManualRequest = value;
      break;
    }
  }
}

 

void drainageCheckStatus(){
  bool drainageSignal = drainageIsOpen();

  if(DrainageManualRequestValue::DrainageOpen){
    if(drainageSignal){
      _DrainageStatus.SignalOn = StatusResult("Open (manual)", STATUS_OK, PRIORITY_LOW);
      _DrainageStatus.Short = StatusResult("Open", STATUS_OK, PRIORITY_LOW);
    }
    else{
      _DrainageStatus.SignalOn = StatusResult("Error, signal off while manual request is on", STATUS_ERROR, PRIORITY_HIGH);
      _DrainageStatus.Short = StatusResult("Error", STATUS_ERROR, PRIORITY_HIGH);
    }
  }
  else if(DrainageManualRequestValue::DrainageClose){
    if(drainageSignal){
      _DrainageStatus.SignalOn = StatusResult("Error, signal on while manual request is off", STATUS_ERROR, PRIORITY_HIGH);
      _DrainageStatus.Short = StatusResult("Error", STATUS_ERROR, PRIORITY_HIGH);
    }
    else{
      _DrainageStatus.SignalOn = StatusResult("Close (manual)", STATUS_OK, PRIORITY_LOW);
      _DrainageStatus.Short = StatusResult("Close", STATUS_OK, PRIORITY_LOW);
    }
  }
  else{ //Automatic
    if(drainageSignal){
      _DrainageStatus.SignalOn = StatusResult("Open (automatic)", STATUS_OK, PRIORITY_LOW);
      _DrainageStatus.Short = StatusResult("Open", STATUS_OK, PRIORITY_LOW);
    }
    else{
      _DrainageStatus.SignalOn = StatusResult("Close (automatic)", STATUS_OK, PRIORITY_LOW);
      _DrainageStatus.Short = StatusResult("Close", STATUS_OK, PRIORITY_LOW);
    }
  }
}
