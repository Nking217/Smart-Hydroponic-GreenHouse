/////////////////////
/// NOTIFICATIONS ///
/////////////////////

void notifyPumpError(char* errorMsg){
  Serial.print("Pump Error, Pleas cheek the water pump. ");
  Serial.println(errorMsg);
  lcdNotifyError("Pump Error");
//  serverNotifyError(ERROR_PUMP, errorMsg);
}

void notifyOFossetError(){
  Serial.print("OFosset Error, Please cheekc your osmosis fosset.");
}

void notifyNFossetError(){
  Serial.print("NFosset Error, Please cheekc your normal fosset.");
}

void notifyLowBatteryWarning(float voltage){
  Serial.print("Low battery levle. Please charge your battery.");
}

void notifyLowBatteryError(float voltage){
  
}
/*
void showBatteryPrecent(byte percent){
  Serial.print("Battery percemt: ", percent);
}
*/
