/////////////////////////
/// Temprature sensor ///
/////////////////////////

#define DHTTYPE DHT11

DHT dht1(TEMPRATURE_SENSOR1_PIN, DHTTYPE);
DHT dht2(TEMPRATURE_SENSOR2_PIN, DHTTYPE);
DHT dht3(TEMPRATURE_SENSOR3_PIN, DHTTYPE);


void tempratureSensorInit(){
  dht.begin();
}

void tempratureTest(){
  float humi1  = dht1.readHumidity();
  float tempC1 = dht1.readTemperature();

  float humi2  = dht2.readHumidity();
  float tempC2 = dht2.readTemperature();

  float humi3  = dht3.readHumidity();
  float tempC3 = dht3.readTemperature();

  Serial.print("Humidity: ");
  Serial.print(humi1);
  Serial.print("%");

  Serial.print("  |  "); 

  Serial.print("Temperature: ");
  Serial.print(tempC1);
  Serial.print("°C ~ ");
}
