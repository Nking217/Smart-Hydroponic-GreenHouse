

struct StatusResult{
  String StatusText;
  int Priorty;  //STATUS_OK, STATUS_WARNING, STATUS_ERROR
  StatusResult(String statusText, int priorty)
  {
    StatusText = statusText;
    Priorty = priorty;
  }
};





///////////////////////////
// function declarations //
///////////////////////////
void batteryInit();
void batteryTest();
float batteryGetVoltage();

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