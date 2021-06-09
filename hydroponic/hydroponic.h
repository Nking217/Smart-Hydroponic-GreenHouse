

struct StatusResult{
  String StatusText;
  byte Status; //STATUS_OK, STATUS_WARNING, STATUS_ERROR
  byte Priorty;  //PRIORITY_HIGH, PRIORITY_MEDIUM, PRIORITY_LOW
  bool Sent;
  StatusResult()
  {
    StatusText = "";
    Status = 0;
    Priorty = 0;
  }
  StatusResult(String statusText, int status, int priorty)
  {
    StatusText = statusText;
    Priorty = priorty;
    Status = status;
  }
  void print(){
    Serial.print(StatusText);
    Serial.print("  priority:");
    Serial.print(Priorty);
    Serial.print("  Status:");
    Serial.println(Status);
  }
};

////////////////////////////
/// Normal Fosset Status ///
////////////////////////////
class nfossetStatus{
  public:
    StatusResult WaterFlowing;
    StatusResult CurrentOn;
    StatusResult SignalOn;
    StatusResult CanRun;
    StatusResult SuposeToRun;

    StatusResult Short;

    void print(){
      Serial.println("------------------");
      Serial.println("Normal Fosset");
      Serial.print("WaterFlowing:");
      WaterFlowing.print();
      Serial.print("CurrentOn:");
      CurrentOn.print();
      Serial.print("SignalOn:");
      SignalOn.print();
      Serial.print("CanRun:");
      CanRun.print();
      Serial.print("SuposeToRun:");
      SuposeToRun.print();
      Serial.println("------------------");
      Serial.println();
    }
};

nfossetStatus _NfossetStatus;


/////////////////////////////
/// Osmotic Fosset Status ///
/////////////////////////////

class ofossetStatus{
  public:
    StatusResult WaterFlowing;
    StatusResult CurrentOn;
    StatusResult SignalOn;
    StatusResult CanRun;
    StatusResult SuposeToRun;

    StatusResult Short;

    void print(){
      Serial.println("------------------");
      Serial.println("Osmotic Fosset");
      Serial.print("WaterFlowing:");
      WaterFlowing.print();
      Serial.print("CurrentOn:");
      CurrentOn.print();
      Serial.print("SignalOn:");
      SignalOn.print();
      Serial.print("CanRun:");
      CanRun.print();
      Serial.print("SuposeToRun:");
      SuposeToRun.print();
      Serial.println("------------------");
      Serial.println();
    }
};

ofossetStatus _OfossetStatus;

///////////////////
/// Pump Status ///
///////////////////

class PumpStatus{
  public:
    StatusResult WaterFlowing;
    StatusResult CurrentOn;
    StatusResult SignalOn;
    StatusResult CanRun;
    StatusResult SuposeToRun;

    StatusResult Short;

    void print(){
      Serial.println("------------------");
      Serial.println("Pump");
      Serial.print("WaterFlowing:");
      WaterFlowing.print();
      Serial.print("CurrentOn:");
      CurrentOn.print();
      Serial.print("SignalOn:");
      SignalOn.print();
      Serial.print("CanRun:");
      CanRun.print();
      Serial.print("SuposeToRun:");
      SuposeToRun.print();
      Serial.println("------------------");
      Serial.println();
    }
};

PumpStatus _PumpStatus;

///////////////////////
/// Drainage Status ///
///////////////////////

class DrainageStatus{
  public:
    StatusResult SignalOn;

    StatusResult Short;

    void print(){
      Serial.println("------------------");
      Serial.println("Drainage");
      Serial.print("SignalOn:");
      SignalOn.print();
      Serial.println("------------------");
      Serial.println();
    }
};

DrainageStatus _DrainageStatus;

//////////////////////
/// Battery Status ///
//////////////////////

class BatteryStatus{
  public:
    StatusResult Voltage;
    
    StatusResult Short;

    void print(){
      Serial.println("------------------");
      Serial.println("Battery");
      Serial.print("Voltage:");
      Voltage.print();
      Serial.println("------------------");
      Serial.println();
    }
};

BatteryStatus _BatteryStatus;

//////////////////////////
/// Water Hight Status ///
//////////////////////////

class WaterHightStatus{
  public:
    StatusResult WaterLevle;

    StatusResult Short;

    void print(){
      Serial.println("------------------");
      Serial.println("Water Hight");
      Serial.print("WaterLevle:");
      WaterLevle.print();
      Serial.println("------------------");
      Serial.println();
    }
};

WaterHightStatus _WaterHightStatus;

//////////////////////////
/// Temperature Status ///
//////////////////////////

class TemperatureStatus{
  public:
    StatusResult Temperature1;
    StatusResult Temperature2;
    StatusResult Temperature3;
    
    StatusResult Short;

    void print(){
      Serial.println("------------------");
      Serial.println("Temperature");
      Serial.print("Temperature1:");
      Temperature1.print();
      Serial.print("Temperature2:");
      Temperature2.print();
      Serial.print("Temperature3:");
      Temperature3.print();
      Serial.println("------------------");
      Serial.println();
    }
};

TemperatureStatus _TemperatureStatus;

/////////////////////////
/// Humidity Status ///
/////////////////////////

class HumidityStatus{
  public:
    StatusResult Humidity1;
    StatusResult Humidity2;
    StatusResult Humidity3;

    StatusResult Short;

    void print(){
      Serial.println("------------------");
      Serial.println("Temperature");
      Serial.print("Humidity1:");
      Humidity1.print();
      Serial.print("Humidity2:");
      Humidity2.print();
      Serial.print("Humidity3:");
      Humidity3.print();
      Serial.println("------------------");
      Serial.println();
    }
};

HumidityStatus _HumidityStatus;

////////////////////////
/// Turbidity Status ///
////////////////////////

class TurbidityStatus{
  public:
    StatusResult IsWorking;

    StatusResult Short;
};

TurbidityStatus _TurbidityStatus;

///////////////////////////
// function declarations //
///////////////////////////
void batteryInit();
void batteryTest();
float batteryGetVoltage();
void serialInit();
void test();

void turbidityTest();
void turbidityInit();
int turbidityReadPercent();
bool tubidityIsWorking();

void drainageTest();
void drainageInit();
void drainageOpen();
void drainageClose();

void espInit();
void espTest();
void espNotifyError(int errorID, String error, int priorty);

void lcdInit();
void lcdTest();
void lcdNotifyError(String error);
void lcdShowState();
void lcdShowHomeScreen();
void lcdShowHomeScreenStatus();
void lcdShowStatusScreen();
void lcdShowSetupScreen();
void lcdHandleTouch();

void nfossetTest();
void nfossetInit();
void nfossetOpen();
void nfossetClose();
void nfossetManualControl();
bool nfossetCanRun();

void nfossetFlowInit();
void nfossetFlowTest();
bool nfossetIsWaterFlowing();

void nfossetCurrentInit();
void nfossetCurrentTest();
bool nfossetIsRunning();
int _NfossetSignalOn;

void notifyPumpError();
void notifyDrainageError();
void notifyLowBatteryWarning(float voltage);
void notifyLowBatteryError(float voltage);
void showBatteryPrecent(byte percent);

void ofossetTest();
void ofossetInit();
void ofossetOpen();
void ofossetClose();
void ofossetManualControl();
bool ofossetCanRun();

void ofossetFlowInit();
void ofossetFlowTest();
bool ofossetIsWaterFlowing();

void ofossetCurrentInit();
void ofossetCurrentTest();
bool ofossetIsRunning();

void pumpTest();
void pumpInit();
void pumpStart();
void pumpStop();
void pumpManualControl();

void pumpCurrentInit();
void pumpCurrentTest();
bool pumpIsRunning();

void solenoidInit();
void solenoidTest();
void solenoidOpen();
void solenoidClose();

void waterHightTest();
void waterHightInit();
int waterHightRead();

void espInit();

void waterHightSensorInit();
void waterHightSensorTest();
