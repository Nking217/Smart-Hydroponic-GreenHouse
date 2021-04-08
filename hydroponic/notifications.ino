/////////////////////
/// NOTIFICATIONS ///
/////////////////////

void notifyPumpError(){
  Serial.print("Pump Error, Pleas cheek the water pump.");
  lcdNotifyError("Pump Error");
  espNotifyError(ERROR_PUMP, "Pump Error", PRIORITY_HIGH);
}

void notifyDrainageError(){
  Serial.print("Drainage Error, Please cheekc your drainage fosset.");
}

void notifyLowBatteryWarning(float voltage){
  Serial.print("Low battery levle. Please charge your battery.");
}

void notifyLowBatteryError(float voltage){
  
}

void showBatteryPrecent(byte percent){
  Serial.print("Battery percemt: ", percent);
}
