////////////////////////////////////////////////////////////////////////////
/////// Smart Hydroponic Greenhouse - Made by Nave Sasoni 2020-2021 ///////
////////////////////////////////////////////////////////////////////////////

#include "DHT.h"
#include <SoftwareSerial.h>
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
#define TEMPERATURE_SENSOR1_PIN A10
#define TEMPERATURE_SENSOR2_PIN A11
#define TEMPERATURE_SENSOR3_PIN A12
#define LIGHT_SENSOR1_PIN A3
#define LIGHT_SENSOR2_PIN A7
#define LIGHT_SENSOR3_PIN A8
//ESP TX Pin 14, ESP RX Pin 15


#define WATER_TANK_TOTAL_HIGHT 50 //50 cm water tank height

#define ERROR -1
#define MINIMUM_WATER_FOR_PUMP 20 //MINIMUM_WATER_IN_TANK
#define MAXIMUM_WATER_IN_TANK 90

#define LCD_PAGE_HOME 1 //
#define LCD_PAGE_STATUS 2
#define LCD_PAGE_CONFIG 3
#define LCD_PAGE_ABOUT 4    

#define STATUS_SUBPAGE_MAIN 0
#define STATUS_SUBPAGE_NFOSSET 1
#define STATUS_SUBPAGE_OFOSSET 2
#define STATUS_SUBPAGE_PUMP 3
#define STATUS_SUBPAGE_DRAINAGE 4
#define STATUS_SUBPAGE_BATTERY 5
#define STATUS_SUBPAGE_WATER 6
#define STATUS_SUBPAGE_TEMPERATURE 7
#define STATUS_SUBPAGE_HUMIDITY 8
#define STATUS_SUBPAGE_TURBIDITY 9
#define STATUS_SUBPAGE_WIFI 10
#define STATUS_SUBPAGE_SERVER 11


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

#define MINIMUM_TEMPERATURE_FOR_WARNING 7
#define MINIMUM_TEMPERATURE_FOR_ERROR 2
#define MAXIMUM_TEMPERATURE_FOR_WARNING 32
#define MAXIMUM_TEMPERATURE_FOR_ERROR 40

#define MINIMUM_HUMIDITY_FOR_WARNING 50
#define MINIMUM_HUMIDITY_FOR_ERROR 30
#define MAXIMUM_HUMIDITY_FOR_WARNING 90
#define MAXIMUM_HUMIDITY_FOR_ERROR 95


#define TIMEOUT_FOR_LAST_VALUD_WATER_HEIGHT 20000

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
int _Lcd_Status; // LCD_PAGE_HOME, LCD_PAGE_STATUS, LCD_PAGE_CONFIG, LCD_PAGE_ABOUT.
int _Lcd_Status_Subpage; 



void setup(){
  serialInit();
  temperatureSensorInit();
  ofossetInit();
  nfossetInit();
  pumpInit();
  drainageInit();
  turbidityInit();
  pumpCurrentInit();
  waterHightSensorInit();
  lcdInit();
  espInit();
  eepromInit();
  lcdShowHomeScreen();
  pumpManualControl(PumpManualRequestValue::PumpAutomatic);
  ofossetManualControl(FossetManualRequestValue::FossetAutomatic);
  nfossetManualControl(FossetManualRequestValue::FossetAutomatic);
}

void loop(){
  checkStatus();
  lcdHandleTouch();
  if(_Lcd_Status == LCD_PAGE_HOME){
    lcdShowHomeScreenStatus();
  }
  else if(_Lcd_Status == LCD_PAGE_STATUS){
    lcdShowStatusScreenStatus();
  }
  //_PumpStatus.Short.print();
  //waterHightSensorTest();
  /*
  Serial.print("--");
  Serial.println(waterHightRead());
  delay(2000);
  */
  //turbidityTest();
  //lcdHandleTouch();
  //getTemperatureStatusResult(int 1 bool true);
  //Serial.print(_Lcd_Status);
  //test();
  mainLogic(); //Automation 
  //tempratureSendSensorData();
}

void mainLogic(){
  if(Serial.available()){
    char c = Serial.read();
    if(c = 's'){
      _NfossetStatus.print();
      _OfossetStatus.print();
      _PumpStatus.print();
      _DrainageStatus.print();
      _BatteryStatus.print();
      _WaterHightStatus.print();
      _TemperatureStatus.print();
      _HumidityStatus.print();
      //_TurbidityStatus.print();
    }
  }
  
  espReconnect();
  serverReconnect();
  
  int h = waterHightRead();
  if(h < MINIMUM_WATER_FOR_PUMP){ //The water is bellow the minimum - pump stop and the fosset is open
    pumpStop();
    if (_OFossetManualRequest == FossetManualRequestValue::FossetAutomatic) ofossetOpen(); //Manual control on the fosset - FossetManualRequestValue::FossetAutomatic
  }
  else if(h >= MINIMUM_WATER_FOR_PUMP){ // The water is above the minimum ammount to start the pump again. 
    if (_PumpManualRequest == PumpManualRequestValue::PumpAutomatic) pumpStart(); //Manual control on the pump - PumpManualRequestValue::PumpAutomatic)
  }
  if(h >= MAXIMUM_WATER_IN_TANK){ //The water levle has reached the maximum, the fosset is closing (the pump keeps working)
    ofossetClose();
  }

  //Cheeks if the pump is really running when he gets the command to start.//
  if(pumpIsSignalOn() && !pumpIsCurrentOn()){  
    pumpStop();
  }
/*
  float v = batteryGetVoltage();
  float p = batteryGetPercent();

  if(v < MINIMUM_VOLTAGE_FOR_WARNING){
    pumpStop();
    ofossetClose();
    nfossetClose();
    drainageClose();
  }

  //check battery volateg
  float b = batteryGetVoltage();
  if(b < MINIMUM_VOLTAGE_FOR_ERROR){
    notifyLowBatteryError(b);
  }
  else(b < MINIMUM_VOLTAGE_FOR_WARNING){
    pumpStop();
    ofossetClose();
    drainageClose();
    notifyLowBatteryWarning(b);
  }
  */
}
void test()
{
//  TEMPERATURETest();
//  ofossetTest();
//  nfossetTest();
//
//  pumpTest();
//  drainageTest();
//  turbidityTest();
//  waterHightTest();
//  pumpCurrentTest();
//  lcdTest();
//  espTest();
}

void checkStatus(){
  nfossetCheckStatus();
  ofossetCheckStatus();
  pumpCheckStatus();
  drainageCheckStatus();
  batteryCheckStatus();
  temperatureCheckStatus();
  humidityCheckStatus();
  lcdHandleTouch();
  waterHightCheckStatus();
}
void serialInit(){
  Serial.begin(9600);
}
