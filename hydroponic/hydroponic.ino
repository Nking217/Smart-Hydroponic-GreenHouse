//#include <dht.h>
#include "DHT.h"



#include "hydroponic.h"
#include "ili9488.h"
#define BATTERY_VOLTAGE_TEST_PIN A1
#define PUMP_CURRENT_TEST_PIN A0
#define PUMP_FLOW_TEST_PIN A6
#define NFOSSET_CURRENT_TEST_PIN A4
#define OFOSSET_CURRENT_TEST_PIN A5
#define NFOSSET_FLOW_TEST_PIN A2
#define OFOSSET_FLOW_TEST_PIN A3 
#define PUMP_PIN 6
#define NFOSSET_PIN 8
#define OFOSSET_PIN 9
#define DRAINAGE_PIN 10
#define SOLENOID_PIN 16 //New
#define WATER_HIGH_SWITCH_PIN 12
#define WATER_LOW_SWITCH_PIN 11
#define TURBIDITY_PIN A2
#define TEMPRATURE_SENSOR1_PIN A10
#define TEMPRATURE_SENSOR2_PIN A11
#define TEMPRATURE_SENSOR3_PIN A12
#define LIGHT_SENSOR1_PIN A3
#define LIGHT_SENSOR2_PIN A7
#define LIGHT_SENSOR3_PIN A8
//ESP TX Pin 14, ESP RX Pin 15

#define ERROR -1
#define MINIMUM_WATER_FOR_PUMP 20 //MINIMUM_WATER_IN_TANK
#define MAXIMUM_WATER_IN_TANK 100

#define LCD_PAGE_HOME 1 //
#define LCD_PAGE_STATUS 2
#define LCD_PAGE_SETUP 3


#define MAX_VALUE_FOR_VOLTAGE_TEST 700
#define MAX_BATTERY_VOLTAGE 12
#define MINIMUM_VOLTAGE_FOR_WARNING 10
#define MINIMUM_VOLTAGE_FOR_ERROR 9 //Test this later 
#define ERROR_PUMP 1

#define PRIORITY_HIGH 1
#define PRIORITY_MEDIUM 2
#define PRIORITY_LOW 3

#define STATUS_OK 1
#define STATUS_WARNING 2
#define STATUS_ERROR 3


#define STATUS_NORMAL_COLOR BLACK
#define STATUS_OK_COLOR GREEN
#define STATUS_WARNING_COLOR YELLOW
#define STATUS_ERROR_COLOR RED

enum DrainageManualRequestValue{
  DrainageOpen,
  DrainageClose,
  DrainageAutomatic
};

enum FossetManualRequestValue{
  FossetOpen,
  FossetClose,
  FossetAutomatic
};

enum PumpManualRequestValue{
  PumpStart,
  PumpStop,
  PumpAutomatic
};

//M2M_LM75A _Lm75a; //Temp Sensor
DrainageManualRequestValue _DrainageManualRequest;

FossetManualRequestValue _OFossetManualRequest;
FossetManualRequestValue _NFossetManualRequest;

PumpManualRequestValue _PumpManualRequest;
bool _IsPumpRunning;
bool _PumpError;
bool _VoltageError;
//int temperatureGet();
int _Lcd_Status; // LCD_PAGE_HOME, LCD_PAGE_STATUS, LCD_PAGE_SETUP.


void setup(){
  serialInit();
  //temperatureInit();
  ofossetInit();
  nfossetInit();
  pumpInit();
  drainageInit();
  turbidityInit();
  pumpCurrentInit();
  lcdInit();
  espInit();
  eepromInit();
  lcdShowHomeScreen();
}

void loop(){
  nfossetCheckStatus();
  ofossetCheckStatus();
  pumpCheckStatus();
  drainageCheckStatus();
  batteryCheckStatus();
  lcdShowHomeScreenStatus();
  lcdHandleTouch();
  test();
  //mainLogic(); //Automation 
}



void mainLogic() //
{
  espReconnect();
//  serverReconnect();
  
  int h = waterHightRead();
  if(h < MINIMUM_WATER_FOR_PUMP){ //The water is bellow the minimum - pump stop and the fosset is open
    pumpStop();
    if (_OFossetManualRequest == FossetManualRequestValue::FossetAutomatic) ofossetOpen(); //Manual control on the fosset
  }
  else if(h >= MINIMUM_WATER_FOR_PUMP){ // The water is above the minimum ammount to start the pump again. 
    if (_PumpManualRequest == PumpManualRequestValue::PumpAutomatic) pumpStart(); //Manual control on the pump
  }
  else if(h >= MAXIMUM_WATER_IN_TANK){ //The water levle has reached the maximum, the fosset is closing (the pump keeps working)
    nfossetClose();
  }
  
  //Cheeks if the pump is really running when he gets the command to start.//
  if(_IsPumpRunning && !pumpIsRunning()){ 
    _PumpError = true; 
    pumpStop();
    notifyPumpError();
  }
  else{
    _PumpError = false;
  }

  //check battery volateg
  float b = batteryGetVoltage();
  if(b < MINIMUM_VOLTAGE_FOR_ERROR){
    notifyLowBatteryError(b);
  }
  else if(b < MINIMUM_VOLTAGE_FOR_WARNING){
    _VoltageError = true;
    pumpStop();
    ofossetClose();
    drainageClose();
    notifyLowBatteryWarning(b);
  }
  else
  {
    _VoltageError = false;
  }
}


void test()
{
  tempratureTest();
//  ofossetTest();
//  nfossetTest();
//  pumpTest();
//  drainageTest();
//  turbidityTest();
//  waterHightTest();
//  pumpCurrentTest();
//  lcdTest();
//  espTest();
}

void serialInit(){
  Serial.begin(9600);
}


/////////////////////////
/// Temperature LM75a ///
///////////////////////// //Switch the tempreature sensor or fix the code...

/* //fix this shit
void temperatuereTest(){
  Serial.println("testing Temperature sensor");
  if(_Lm75a.isConnected()){
    Serial.println ("temperture sensor connected");
    int temp = temperatureGet();
    if(temp >0 && temp<40)
      Serial.println(temp);
  }
  else
  {
    Serial.println("ERROR: tempretature sensor not connected");
  }
}

bool temperatureInit(){
  _Lm75a.begin();
  return _Lm75a.isConnected();
}
int temperatureGet(){
  return _Lm75a.getTemperature();
}
*/
