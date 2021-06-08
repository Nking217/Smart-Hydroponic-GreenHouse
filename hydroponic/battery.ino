///////////////
/// BATTERY ///
///////////////

void batteryInit(){
  pinMode(BATTERY_VOLTAGE_TEST_PIN, INPUT);
}

void batteryTest(){
  
}

float batteryGetVoltage(){ 
  int d = analogRead(BATTERY_VOLTAGE_TEST_PIN);
  return map(d, 0, MAX_VALUE_FOR_VOLTAGE_TEST, 0, MAX_BATTERY_VOLTAGE);
}

float batteryGetPercent(){ 
  int d = analogRead(BATTERY_VOLTAGE_TEST_PIN);
  return map(d, 0, MAX_VALUE_FOR_VOLTAGE_TEST, 0, 100);
}

void batteryCheckStatus(){
  int batteryVoltage = batteryGetVoltage();
  int batteryPercent = batteryGetPercent();

  String percent = String(batteryPercent);
  
  char buffer[80];
  
  if(batteryVoltage < MINIMUM_VOLTAGE_FOR_ERROR){
    sprintf(buffer, "Error, %dv(%d\%) battery levle is too low", batteryVoltage, batteryPercent); 
    _BatteryStatus.Voltage = StatusResult(String(buffer), STATUS_ERROR, PRIORITY_HIGH);
    sprintf(buffer, "%dv(%d%%)", batteryVoltage, batteryPercent);
    _BatteryStatus.Short = StatusResult(String(buffer), STATUS_ERROR, PRIORITY_HIGH);
  }
  else if(batteryVoltage < MINIMUM_VOLTAGE_FOR_WARNING){
    sprintf(buffer, "Warning, %dv(%d\%) battery levle is low", batteryVoltage, batteryPercent);
    _BatteryStatus.Voltage = StatusResult(String(buffer), STATUS_WARNING, PRIORITY_MEDIUM);
    sprintf(buffer, "%dv(%d%%)", batteryVoltage, batteryPercent);
    _BatteryStatus.Short = StatusResult(String(buffer), STATUS_WARNING, PRIORITY_MEDIUM);
  }
  else{ //Battery levle is normal
    sprintf(buffer, "%dv(%d%%)", batteryVoltage, batteryPercent);
    _BatteryStatus.Voltage = StatusResult(String(buffer), STATUS_OK, PRIORITY_LOW);
    _BatteryStatus.Short = StatusResult(String(buffer), STATUS_OK, PRIORITY_LOW);

  }
}
