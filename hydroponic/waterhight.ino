/////////////////////
///  WATER HIGHT  ///
/////////////////////

SoftwareSerial mySerial(12,11); // RX, TX
unsigned char data[4]={};
float distance;

void waterHightSensorInit(){
  Serial.begin(57600);
  mySerial.begin(9600); 
}

void waterHightSensorTest(){
     do{
     for(int i=0;i<4;i++)
     {
       data[i]=mySerial.read();
     }
  }while(mySerial.read()==0xff);

  mySerial.flush();

  if(data[0]==0xff)
    {
      int sum;
      sum=(data[0]+data[1]+data[2])&0x00FF;
      if(sum==data[3])
      {
        distance=(data[1]<<8)+data[2];
        if(distance>30)
          {
           Serial.print("distance=");
           Serial.print(distance/10);
           Serial.println("cm");
          }else 
             {
               Serial.println("Below the lower limit");
             }
      }else Serial.println("ERROR");
     }
     delay(100);
}


int _LastValidWaterHightValue = -2;
long _LastValidWaterHightTimestamp= 0;

int waterHightRead(){
  int value = ultrasonicReadDistance();

  if (value == -1) //too close
  {
    _LastValidWaterHightValue = 100;
    _LastValidWaterHightTimestamp = millis();
  }
  else if (value != -2 && value > WATER_TANK_TOTAL_HIGHT*10) //bigger then the tank - cannot be possible
  {
    _LastValidWaterHightValue = -2;
    _LastValidWaterHightTimestamp = millis();
  }
  else if (value != -2)
  {
    _LastValidWaterHightValue = ((WATER_TANK_TOTAL_HIGHT*10 - value)*10)/WATER_TANK_TOTAL_HIGHT;   //calculate percentage
    _LastValidWaterHightTimestamp = millis();
  } 
  if (millis() - _LastValidWaterHightTimestamp > TIMEOUT_FOR_LAST_VALUD_WATER_HEIGHT)
  {
    _LastValidWaterHightValue = - 2;
  }
  return _LastValidWaterHightValue;
}

int ultrasonicReadDistance(){
  mySerial.flush();
  mySerial.write(HIGH);
  for(int i=0;i<4;i++)
  {
    data[0]=mySerial.read();
    if (data[0] == 0xff)
    {
      for(int i=1;i<4;i++)
      {
        data[i]=mySerial.read();
      }
      break;
    }
  }
  /*
  do{
     for(int i=0;i<4;i++)
     {
       data[i]=mySerial.read();
     }
  }while(mySerial.read()==0xff);
*/
  mySerial.flush();

  if(data[0]==0xff)
  {
    int sum;
    sum=(data[0]+data[1]+data[2])&0x00FF;
    if(sum==data[3])
    {
      distance=(data[1]<<8)+data[2];
      if(distance>30)
        return (distance);
      else 
        return -1;  //too close
    }
    else 
    {
      return -2;    //error
    }
  }
}

/*

void waterHightTest(){
  //Sensor test
  Serial.print("Water low sensor:"); 
  Serial.println(digitalRead(WATER_LOW_SWITCH_PIN)); 
  Serial.print("Water high sensor:"); 
  Serial.println(digitalRead(WATER_HIGH_SWITCH_PIN)); 
  delay(1000);
  //Function test
  //Serial.print("Water hight test:"); 
  //Serial.println(waterHightRead()); 
}

void waterHightInit(){
  pinMode(WATER_HIGH_SWITCH_PIN, INPUT);
  pinMode(WATER_LOW_SWITCH_PIN, INPUT);
}

int waterHightRead(){
  return 15;
}

int waterHightRead(){
  bool waterInLow = !digitalRead(WATER_LOW_SWITCH_PIN); //* off/down = 1,  on/up = 0
  bool waterInHigh = !digitalRead(WATER_HIGH_SWITCH_PIN);  //* true 1 , false 0.
  if(!waterInLow){
    return 0;
  }
  else if(waterInHigh){
    return 100;
  }
  else if (!waterInHigh && waterInLow){
    return 50;
  }
  else {
    return ERROR;
  }
}
*/
void waterHightCheckStatus(){
  int WaterHight = waterHightRead();
  Serial.println(WaterHight);
  String WaterLevle = String(WaterHight);
  //char buffer[80]
  
  if(WaterHight == -2){
    _WaterHightStatus.WaterLevle = StatusResult("Error, check the water hight sensor", STATUS_ERROR, PRIORITY_HIGH);
    _WaterHightStatus.Short = StatusResult("Error", STATUS_ERROR, PRIORITY_HIGH);
  }
  else if(WaterHight >= MAXIMUM_WATER_IN_TANK){
    _WaterHightStatus.WaterLevle = StatusResult("Error, water levle is too high", STATUS_ERROR, PRIORITY_HIGH);
    _WaterHightStatus.Short = StatusResult("Error", STATUS_ERROR, PRIORITY_HIGH);
  }
  else if(WaterHight <= MINIMUM_WATER_FOR_PUMP){
    _WaterHightStatus.WaterLevle = StatusResult("Error, water levle too low for pump", STATUS_ERROR, PRIORITY_HIGH);
    _WaterHightStatus.Short = StatusResult("Error", STATUS_ERROR, PRIORITY_HIGH);
  }
  else{
    _WaterHightStatus.WaterLevle = StatusResult(WaterLevle, STATUS_OK, PRIORITY_LOW);
    _WaterHightStatus.Short = StatusResult(WaterLevle, STATUS_OK, PRIORITY_LOW);
  }
}
