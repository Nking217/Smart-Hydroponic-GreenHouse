/////////////////////////
/// TEMPERATURE sensor ///
/////////////////////////

#define DHTTYPE DHT11

DHT* _TempHumiSensors[3];


void temperatureSensorInit(){
  _TempHumiSensors[0] = new DHT(TEMPERATURE_SENSOR1_PIN, DHTTYPE);
  _TempHumiSensors[1] = new DHT(TEMPERATURE_SENSOR2_PIN, DHTTYPE);
  _TempHumiSensors[2] = new DHT(TEMPERATURE_SENSOR3_PIN, DHTTYPE);
  
  _TempHumiSensors[0]->begin();
  _TempHumiSensors[1]->begin();
  _TempHumiSensors[2]->begin();
}

void temperatureTest(){
  for(int i = 1; i <= 3; i++){
     float humi  = getHumidity(i);
     float tempC = getTemperature(i);

     Serial.println("Humidity: ");
     Serial.print(humi);
     Serial.print("%");
 
     Serial.print("  |  "); 

     Serial.print("Temperature: ");
     Serial.print(tempC);
     Serial.print("°C ~ ");
  }
}

float getTemperature(byte sensorId){
  return _TempHumiSensors[sensorId-1]->readTemperature();
}

float getHumidity(byte sensorId){
  return _TempHumiSensors[sensorId-1]->readHumidity();
}

void temperatureCheckStatus(){
  int temperature1 = getTemperature(1);
  int temperature2 = getTemperature(2);
  int temperature3 = getTemperature(3);

  int avgTemperature = temperature1 + temperature2 + temperature3 / 3;
  
  _TemperatureStatus.Temperature1 = getTemperatureStatusResult(temperature1,false);
  _TemperatureStatus.Temperature2 = getTemperatureStatusResult(temperature2,false);
  _TemperatureStatus.Temperature3 = getTemperatureStatusResult(temperature3,false);

  _TemperatureStatus.Short = getTemperatureStatusResult(avgTemperature, true);
}

StatusResult getTemperatureStatusResult(int value, bool shortStatus){
  
  char buffer[80];
  //Temperature printing not working -- fix this
  if(shortStatus) sprintf(buffer, "%f°C", value);
    
  if (value > MAXIMUM_TEMPERATURE_FOR_ERROR){
    if(!shortStatus) sprintf(buffer, "Error, Temperature %d2°C is too high", value); 
    return StatusResult(String(buffer), STATUS_ERROR, PRIORITY_HIGH);
  }
  else if(value > MAXIMUM_TEMPERATURE_FOR_WARNING){
    if(!shortStatus) sprintf(buffer, "Warning, Temperature %d2°C is high", value); 
    return StatusResult(String(buffer), STATUS_WARNING, PRIORITY_MEDIUM);
  }
  else if(value < MINIMUM_TEMPERATURE_FOR_ERROR){
    if(!shortStatus) sprintf(buffer, "Warning, Temperature %d2°C is high", value); 
    return StatusResult(String(buffer), STATUS_ERROR, PRIORITY_HIGH);
  }
  else if(value < MINIMUM_TEMPERATURE_FOR_WARNING){
    if(!shortStatus) sprintf(buffer, "Warning, Temperature %d2°C is low", value); 
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
