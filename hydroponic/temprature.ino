/////////////////////////
/// Temprature sensor ///
/////////////////////////

#define DHTTYPE DHT11

DHT* _TempHumiSensors[3];


void tempratureSensorInit(){
  _TempHumiSensors[0] = new DHT(TEMPRATURE_SENSOR1_PIN, DHTTYPE);
  _TempHumiSensors[1] = new DHT(TEMPRATURE_SENSOR2_PIN, DHTTYPE);
  _TempHumiSensors[2] = new DHT(TEMPRATURE_SENSOR3_PIN, DHTTYPE);
  
  _TempHumiSensors[0]->begin();
  _TempHumiSensors[1]->begin();
  _TempHumiSensors[2]->begin();
}

void tempratureTest(){
  for(int i = 1; i <= 3; i++){
     float humi  = getHumidity(i);
     float tempC = getTemprature(i);

     Serial.print("Humidity: ");
     Serial.print(humi);
     Serial.print("%");
 
     Serial.print("  |  "); 

     Serial.print("Temperature: ");
     Serial.print(tempC);
     Serial.print("°C ~ ");
  }
}

float getTemprature(byte sensorId){
  return _TempHumiSensors[sensorId-1]->readTemperature();
}

float getHumidity(byte sensorId){
  return _TempHumiSensors[sensorId-1]->readHumidity();
}

void tempratureCheckStatus(){
  float temprature1 = getTemprature(1);
  float temprature2 = getTemprature(2);
  float temprature3 = getTemprature(3);

  float avgTemprature = temprature1 + temprature2 + temprature3 / 3;
  
  _TempratureStatus.Temprature1 = getTempratureStatusResult(temprature1,false);
  _TempratureStatus.Temprature2 = getTempratureStatusResult(temprature2,false);
  _TempratureStatus.Temprature3 = getTempratureStatusResult(temprature3,false);

  _TempratureStatus.Short = getTempratureStatusResult(avgTemprature, true);
}

StatusResult getTempratureStatusResult(float value, bool shortStatus){
  
  char buffer[80];

  if(shortStatus) sprintf(buffer, "%f2°C", value);
    
  if (value > MAXIMUM_TEMPRATURE_FOR_ERROR){
    if(!shortStatus) sprintf(buffer, "Error, Temprature %f2°C is too high", value); 
    return StatusResult(String(buffer), STATUS_ERROR, PRIORITY_HIGH);
  }
  else if(value > MAXIMUM_TEMPRATURE_FOR_WARNING){
    if(!shortStatus) sprintf(buffer, "Warning, Temprature %f2°C is high", value); 
    return StatusResult(String(buffer), STATUS_WARNING, PRIORITY_MEDIUM);
  }
  else if(value < MINIMUM_TEMPRATURE_FOR_ERROR){
    if(!shortStatus) sprintf(buffer, "Warning, Temprature %f2°C is high", value); 
    return StatusResult(String(buffer), STATUS_ERROR, PRIORITY_HIGH);
  }
  else if(value < MINIMUM_TEMPRATURE_FOR_WARNING){
    if(!shortStatus) sprintf(buffer, "Warning, Temprature %f2°C is low", value); 
    return StatusResult(String(buffer), STATUS_WARNING, PRIORITY_MEDIUM);
  }
  else{
    return StatusResult(String(buffer), STATUS_OK, PRIORITY_LOW);
  }
}

void humidityCheckStatus(){ 
  float humidity1 = getHumidity(1);
  float humidity2 = getHumidity(2);
  float humidity3 = getHumidity(3);

  float avgHumidity = humidity1 + humidity2 + humidity3 / 3;

  _HumidityStatus.Humidity1 = getHumidityStatusResult(humidity1, false);
  _HumidityStatus.Humidity2 = getHumidityStatusResult(humidity2, false);
  _HumidityStatus.Humidity3 = getHumidityStatusResult(humidity3, false);
  
}


StatusResult getHumidityStatusResult(float value, bool shortStatus){
  
  char buffer[80];

  if(shortStatus) sprintf(buffer, "%f2%", value);
    
  if (value > MAXIMUM_HUMIDITY_FOR_ERROR){
    if(!shortStatus) sprintf(buffer, "Error, Humidity %f2% is too high", value); 
    return StatusResult(String(buffer), STATUS_ERROR, PRIORITY_HIGH);
  }
  else if(value > MAXIMUM_HUMIDITY_FOR_WARNING){
    if(!shortStatus) sprintf(buffer, "Warning, Humidity %f2% is high", value); 
    return StatusResult(String(buffer), STATUS_WARNING, PRIORITY_MEDIUM);
  }
  else if(value < MINIMUM_HUMIDITY_FOR_ERROR){
    if(!shortStatus) sprintf(buffer, "Warning, Humidity %f2% is high", value); 
    return StatusResult(String(buffer), STATUS_ERROR, PRIORITY_HIGH);
  }
  else if(value < MINIMUM_HUMIDITY_FOR_WARNING){
    if(!shortStatus) sprintf(buffer, "Warning, Humidity %f2% is low", value); 
    return StatusResult(String(buffer), STATUS_WARNING, PRIORITY_MEDIUM);
  }
  else{
    return StatusResult(String(buffer), STATUS_OK, PRIORITY_LOW);
  }
}
