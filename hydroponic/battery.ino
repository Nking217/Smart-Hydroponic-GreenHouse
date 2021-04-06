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


StatusResult batteryStatus_Short(){
  String batteryPercent = String(batteryGetPercent()) + "%";
  float batteryVoltage = batteryGetVoltage();
  if(batteryGetVoltage > MINIMUM_VOLTAGE_FOR_WARNING)
    return StatusResult(batteryPercent, STATUS_OK);
  else if(batteryGetVoltage > MINIMUM_VOLTAGE_FOR_ERROR)
    return StatusResult("Warning (" + batteryPercent + ")", STATUS_WARNING);
  else
    return StatusResult("Error", STATUS_ERROR);
}
