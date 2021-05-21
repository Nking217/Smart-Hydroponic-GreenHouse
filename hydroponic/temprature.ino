/////////////////////////
/// Temprature sensor ///
/////////////////////////

void tempratureSensorInit(){
  
}

void tempratureTest(){
  int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temprature: ");
  Serial.println(DHT.temperature);
   
}
