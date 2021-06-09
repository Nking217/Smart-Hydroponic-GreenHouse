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

  float avgTemperature = (temperature1 + temperature2 + temperature3) / 3;
  
  String oldStatus1 = _TemperatureStatus.Temperature1.StatusText;
  bool oldSent1 = _TemperatureStatus.Temperature1.Sent;
  _TemperatureStatus.Temperature1 = getTemperatureStatusResult(temperature1,false);
  if (oldStatus1 != _TemperatureStatus.Temperature1.StatusText)
    _TemperatureStatus.Temperature1.Sent = false;
  else
    _TemperatureStatus.Temperature1.Sent = oldSent1;

  String oldStatus2 = _TemperatureStatus.Temperature2.StatusText;
    bool oldSent2 = _TemperatureStatus.Temperature2.Sent;
    _TemperatureStatus.Temperature2 = getTemperatureStatusResult(temperature2,false);
    if (oldStatus2 != _TemperatureStatus.Temperature2.StatusText)
      _TemperatureStatus.Temperature2.Sent = false;
    else
      _TemperatureStatus.Temperature2.Sent = oldSent2;
  
  String oldStatus3 = _TemperatureStatus.Temperature3.StatusText;
    bool oldSent3 = _TemperatureStatus.Temperature3.Sent;
    _TemperatureStatus.Temperature3 = getTemperatureStatusResult(temperature3,false);
    if (oldStatus3 != _TemperatureStatus.Temperature3.StatusText)
      _TemperatureStatus.Temperature3.Sent = false;
    else
      _TemperatureStatus.Temperature3.Sent = oldSent3;

  //_TemperatureStatus.Temperature2 = getTemperatureStatusResult(temperature2,false);
  // _TemperatureStatus.Temperature3 = getTemperatureStatusResult(temperature3,false);

  _TemperatureStatus.Short = getTemperatureStatusResult(avgTemperature, true);
}

StatusResult getTemperatureStatusResult(int value, bool shortStatus){
  
  char buffer[80];
  //Temperature printing not working -- fix this
  if(shortStatus){
    sprintf(buffer, "%dC", value);
  }

    
  if (value > MAXIMUM_TEMPERATURE_FOR_ERROR){
    if(!shortStatus) sprintf(buffer, "Error, Temperature %dC is too high", value); 
    return StatusResult(String(buffer), STATUS_ERROR, PRIORITY_HIGH);
  }
  else if(value > MAXIMUM_TEMPERATURE_FOR_WARNING){
    if(!shortStatus) sprintf(buffer, "Warning, Temperature %dC is high", value); 
    return StatusResult(String(buffer), STATUS_WARNING, PRIORITY_MEDIUM);
  }
  else if(value < MINIMUM_TEMPERATURE_FOR_ERROR){
    if(!shortStatus) sprintf(buffer, "Warning, Temperature %dC is high", value); 
    return StatusResult(String(buffer), STATUS_ERROR, PRIORITY_HIGH);
  }
  else if(value < MINIMUM_TEMPERATURE_FOR_WARNING){
    if(!shortStatus) sprintf(buffer, "Warning, Temperature %dC is low", value); 
    return StatusResult(String(buffer), STATUS_WARNING, PRIORITY_MEDIUM);
  }
  else{
    return StatusResult(String(buffer), STATUS_OK, PRIORITY_LOW);
  }
}

void humidityCheckStatus(){ 
  int humidity1 = getHumidity(1);
  int humidity2 = getHumidity(2);
  int humidity3 = getHumidity(3);

  int avgHumidity = (humidity1 + humidity2 + humidity3) / 3;

  String oldStatus1 = _HumidityStatus.Humidity1.StatusText;
  bool oldSent1 = _HumidityStatus.Humidity1.Sent;
  _HumidityStatus.Humidity1 = getHumidityStatusResult(humidity1,false);
  if (oldStatus1 != _HumidityStatus.Humidity1.StatusText)
    _HumidityStatus.Humidity1.Sent = false;
  else
    _HumidityStatus.Humidity1.Sent = oldSent1;

  String oldStatus2 = _HumidityStatus.Humidity2.StatusText;
  bool oldSent2 = _HumidityStatus.Humidity2.Sent;
  _HumidityStatus.Humidity2 = getHumidityStatusResult(humidity2,false);
  if (oldStatus2 != _HumidityStatus.Humidity2.StatusText)
    _HumidityStatus.Humidity2.Sent = false;
  else
    _HumidityStatus.Humidity2.Sent = oldSent2;

  String oldStatus3 = _HumidityStatus.Humidity3.StatusText;
  bool oldSent3 = _HumidityStatus.Humidity3.Sent;
  _HumidityStatus.Humidity3 = getHumidityStatusResult(humidity3,false);
  if (oldStatus3 != _HumidityStatus.Humidity3.StatusText)
    _HumidityStatus.Humidity3.Sent = false;
  else
    _HumidityStatus.Humidity3.Sent = oldSent1;

  //_HumidityStatus.Humidity1 = getHumidityStatusResult(humidity1, false);
  //_HumidityStatus.Humidity2 = getHumidityStatusResult(humidity2, false);
  //_HumidityStatus.Humidity3 = getHumidityStatusResult(humidity3, false);

  
  _HumidityStatus.Short = getHumidityStatusResult(avgHumidity, true);
}


StatusResult getHumidityStatusResult(int value, bool shortStatus){
  
  char buffer[80];

  if(shortStatus) sprintf(buffer, "%d%", value);
    
  if (value > MAXIMUM_HUMIDITY_FOR_ERROR){
    if(!shortStatus) sprintf(buffer, "Error, Humidity %d% is too high", value); 
    return StatusResult(String(buffer), STATUS_ERROR, PRIORITY_HIGH);
  }
  else if(value > MAXIMUM_HUMIDITY_FOR_WARNING){
    if(!shortStatus) sprintf(buffer, "Warning, Humidity %d% is high", value); 
    return StatusResult(String(buffer), STATUS_WARNING, PRIORITY_MEDIUM);
  }
  else if(value < MINIMUM_HUMIDITY_FOR_ERROR){
    if(!shortStatus) sprintf(buffer, "Warning, Humidity %d% is high", value); 
    return StatusResult(String(buffer), STATUS_ERROR, PRIORITY_HIGH);
  }
  else if(value < MINIMUM_HUMIDITY_FOR_WARNING){
    if(!shortStatus) sprintf(buffer, "Warning, Humidity %d% is low", value); 
    return StatusResult(String(buffer), STATUS_WARNING, PRIORITY_MEDIUM);
  }
  else{
    return StatusResult(String(buffer), STATUS_OK, PRIORITY_LOW);
  }
}


void tempratureSendSensorData(){
  int temp1 = getTemperature(1);
  int temp2 = getTemperature(2);
  int temp3 = getTemperature(3);

  
  serverSendSensorData(4, 0, temp1);
  serverSendSensorData(5, 0, temp2);
  serverSendSensorData(6, 0, temp3);
}
