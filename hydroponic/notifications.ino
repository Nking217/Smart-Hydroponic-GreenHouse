/////////////////////
/// NOTIFICATIONS ///
/////////////////////

void notifyPumpError(){
  Serial.print("Pump Error, Pleas cheek the water pump.");
  lcdNotifyError("Pump Error");
  espNotifyError(ERROR_PUMP, "Pump Error", PRIORITY_HIGH);
}

void notifyDrainageError(){
  
}

void notifyLowBatteryWarning(float voltage){
  
}

void notifyLowBatteryError(float voltage){
  
}

void showBatteryPrecent(byte percent){
  
}
