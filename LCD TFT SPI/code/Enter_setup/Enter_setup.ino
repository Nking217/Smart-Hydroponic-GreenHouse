#include <Wire.h>     //for I2C communication for RTC
#include "TFT9341.h"  //for LCD screen


#define DS1307_ADDRESS 0x68  //RTC address

#define STATUS_SETUP 1
#define STATUS_RIDDLE 2

#define SETUP_KEY_MAX_INPUT_LENGTH 9


//Time to exit setup screen
#define SETUP_TIME_TO_EXIT 10 //sec - Time from entering setup untill exiting setup

struct RTCTime
{
  byte hour;
  byte minute;
  byte second;
};

int16_t _Screen_OldX;
int16_t _Screen_OldY;
uint8_t _Screen_OldFont;
uint16_t _Screen_OldColor;
uint16_t _Screen_OldBackground;

//array defining the key to enter setup
//the screen is devided to 9 squeres, each squere gets a number
//the array is the defines the path of the key on the touch pad
int _SetupKey[SETUP_KEY_MAX_INPUT_LENGTH] = {1,2,3,5,7,8,9,-1,-1};

int _SetupKeyInput[SETUP_KEY_MAX_INPUT_LENGTH];
int _SetupKeyInputIndex;
int _SetupKeyInputPrevVal;

//the time when we entered setup mode
RTCTime _SetupEnterTime;

//status of opperation (Riddlle or Setup)
int _Status;   //STATUS_SETUP or STATUS_RIDDLE


void setup() 
{
  //begin I2C communication with TRC
  Wire.begin(DS1307_ADDRESS); 

  //start serial interface
  Serial.begin(9600);

  //initialize lcd screen
  lcd.begin();

  //reset time for setup countdown
  resetTime();

  //clear the setup key input array and all other variables
  for (int i=0; i<SETUP_KEY_MAX_INPUT_LENGTH; i++)
  {
    _SetupKeyInput[i]=-1; //clear values
  }
  _SetupKeyInputIndex = 0; //start from index 0
  _SetupKeyInputPrevVal = -1; //no touch

  //enter ridle screen
  enterRiddelMode();
}

void loop() 
{
 
  //if timeout has expired for setup, go back to riddle screen
  if (_Status == STATUS_SETUP && !checkSetupTimeout()) enterRiddelMode();

  //check if there is relevant input to enter setup mode
  if (readSetupKey())
  {
    Serial.println("Setup key received...");
    for (int i=0; i<_SetupKeyInputIndex; i++)
    {
      Serial.print(" ");
      Serial.print(_SetupKeyInput[i]);
    }
    Serial.println("");

    //check that the key entered is a valid key to enter setup mode
    if (checkSetupKey(_SetupKeyInput))
    {
      //if the key is correct then enter setup mode
      enterSetupMode();
    }
  }


  if (_Status == STATUS_SETUP) 
  {
    showSetupCountdown();
  }
  
  //test();
}


//checks if setup timeout has expired
//returns true if still OK
//returns false if time has expired
bool checkSetupTimeout()
{
  if (_Status != STATUS_SETUP) return false;
  int seccordsPassed = calcSecondsPassed(_SetupEnterTime);
  if (seccordsPassed > SETUP_TIME_TO_EXIT) return false;
  return true;
}




//returns the number of the button pressed
//-1 = not thouching
//0 = finger touching, but non of the button is pressed
//1-9 = the number of the button pressed
int readSetupTouch()
{
  //if already in setup mode then no need to read setup key
  if (_Status == STATUS_SETUP) return -1;
  
  lcdtouch.readxy();
  word x = lcdtouch.readx();
  word y = lcdtouch.ready(); 
  
  if (x==320) return -1;  //not thouching
  
  int row;
  int col;
  
  //check which column was pressed
  if (x>22 && x<91) col = 1;
  else if (x>165 && x<194) col = 2;
  else if (x>228 && x<297) col = 3;
  else col = 0;
  
  //check which row was pressed
  if (y>23 && y<65) row = 1;
  else if (y>99 && y<141) row = 2;
  else if (y>175 && y< 217) row = 3;
  else row = 0;
  
  if (col == 0 || row == 0) return 0;   //non of the buttons pressed
  
  return col+(row-1)*3;
}

void test()
{

  /*
  Serial.println("Testing RTC");
  RTCTime now = readTime();
  printTime(now);
  delay (1000);
  */


  for (int i=1; i<=9; i++)
  {
    showSetupKeyButtons(i, RED);
  }
  
  if (readSetupKey())
  {
    Serial.println("Setup key received...");
    for (int i=0; i<_SetupKeyInputIndex; i++)
    {
      Serial.print(" ");
      Serial.print(_SetupKeyInput[i]);
      showSetupKeyButtons(_SetupKeyInput[i], GREEN);
      delay(100);
    }
    Serial.println("");
    
    if (checkSetupKey(_SetupKeyInput))
    {
      enterSetupMode();
    }
    else
    {
      delay(2000);
    }
  }
}


bool readSetupKey()
{
  int val = readSetupTouch();
  //if finger released touch (prev value had a value, and now there is no touch)
  if (val == -1 && _SetupKeyInputPrevVal!=-1) 
  {
    _SetupKeyInputPrevVal = -1;
    return true;
  }

  //if the current value is different than the prev value
  //then it is relevant to check it
  if (val != _SetupKeyInputPrevVal)
  {
    //if we just started pressing...
    if (_SetupKeyInputPrevVal == -1) 
    {
      //clear the _SetupKeyInput array
      for (int i = 0; i < SETUP_KEY_MAX_INPUT_LENGTH; i++)
      {
        _SetupKeyInput[i] = -1;
      }
      _SetupKeyInputIndex = 0;  //start from the begining  
    }

    if (val != 0)
    {
      //make sure that the current value is not the same as the prev value added to the array
      if (_SetupKeyInputIndex == 0 || val != _SetupKeyInput[_SetupKeyInputIndex-1])
      {
        //add the new value to the array (if it is not yet full)
        if (_SetupKeyInputIndex<SETUP_KEY_MAX_INPUT_LENGTH) _SetupKeyInput[_SetupKeyInputIndex++] = val;
      }
    }
  }

  _SetupKeyInputPrevVal = val; 
  return false;
}

//check if the array representing the key for setup menu is correct, then returns true
bool checkSetupKey(int arr[])
{
  for (int i = 0; i < SETUP_KEY_MAX_INPUT_LENGTH && _SetupKey[i] != -1; i++)
  {
    if (arr[i] == -1) return false;
    if (arr[i] != _SetupKey[i]) return false;
  }
  return true;
}

//shows keypad and option to enter new 4 digits for the riddle
//when pressed enter - save and exit
//when pressed cancel - exit without save
void enterSetupMode()
{
  //reset time to start countdown for exit setup
  _SetupEnterTime = readTime();
  //set screen status
  _Status = STATUS_SETUP;
  //show setup screen
  showSetupScreen();
}

//Show setup of the riddle (4 digits)
//show keypad, Enter, Cancel buttons
void showSetupScreen()
{
  lcd.setColor(BLACK, RED);
  lcd.clrscr();
  lcd.gotoxy(30,30);
  lcd.print("setup mode");
}

void showSetupCountdown()
{
  if (_Status == STATUS_SETUP)
  {
    int seccordsPassed = calcSecondsPassed(_SetupEnterTime);
    int remainingSec = SETUP_TIME_TO_EXIT - seccordsPassed;
    Serial.println(remainingSec);
    lcd.setColor(BLACK, RED);
    lcd.gotoxy(1,1);
    lcd.print(remainingSec);
    lcd.print("   ");
  }
}

void enterRiddelMode()
{
  //set screen status
  _Status = STATUS_RIDDLE;
  //show ridle screen
  showRiddelScreen();
}

//shows the current state of the riddle on screen
//4 sqares with V for the resolved part
void showRiddelScreen()
{
  lcd.setColor(BLACK, GREEN);
  lcd.clrscr();
  lcd.gotoxy(30,30);
  lcd.print("riddle mode");
}

void showSetupKeyButtons(int buttonID, int color)
{
  uint16_t oldColor = lcd.getColor();
  lcd.setColor(color);
  switch(buttonID)
  {
    case 1: lcd.fillRoundRect (12,13,89,62,3); break;
    case 2: lcd.fillRoundRect (115,13,89,62,3); break;
    case 3: lcd.fillRoundRect (218,13,89,62,3); break;
    case 4: lcd.fillRoundRect (12,89,89,62,3); break;
    case 5: lcd.fillRoundRect (115,89,89,62,3); break;
    case 6: lcd.fillRoundRect (218,89,89,62,3); break;
    case 7: lcd.fillRoundRect (12,165,89,62,3); break;
    case 8: lcd.fillRoundRect (115,165,89,62,3); break;
    case 9: lcd.fillRoundRect (218,165,89,62,3); break;
  }
  lcd.setColor(oldColor);
}

//read time from TRC
RTCTime readTime()
{
  //send request to RTC
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(0);
  Wire.endTransmission();
  
  //read 7 words from RTC
  Wire.requestFrom(DS1307_ADDRESS,7);

  RTCTime val;  //this is the Date-Time value that will be returned

  //if there is data available to read from RTC
  if(Wire.available())
  {
    val.second = bcdToDec(Wire.read()&0b1111111);   //translate second (discard CH bit 7)
    val.minute = bcdToDec(Wire.read());   //translate minute
    val.hour = bcdToDec(Wire.read());  //translate hour (discard the 12/24 sign bit6 becatse it is set to 0 on setupTime)
  }
  return val;
}

void resetTime()
{
  RTCTime now;
  now.second=0;
  now.minute=0;
  now.hour=0;
  setupTime(now);
}

//set the date and time on the RTC
void setupTime(RTCTime val) 
{
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(0x0);
  Wire.write(decToBcd(val.second));
  Wire.write(decToBcd(val.minute));
  Wire.write(decToBcd(val.hour));
  Wire.endTransmission();
}

//print time to serial port
void printTime(RTCTime now)
{
  Serial.print(now.hour, DEC);
  Serial.print(':');
  Serial.print(now.minute, DEC);
  Serial.print(':');
  Serial.print(now.second, DEC);
  Serial.println();
}

//translate Decimal to bcd
byte decToBcd(byte val)
{
  val= val/10*16 + val%10 ;
  return val;
}

//translate bcd to Decimal
byte bcdToDec(byte val)
{
  val=val/16*10 + val%16;
  return (val);
}

//calculate the number of secords then number of sconds passed from the specified time untill now
int calcSecondsPassed(RTCTime startTime)
{
  int startTotalSecords = calcSecords(startTime);
  RTCTime now = readTime();
  int nowTotalSeconds = calcSecords(now);
  return nowTotalSeconds-startTotalSecords;
}

//calculates the number of total secords since we started the clock
int calcSecords(RTCTime val)
{
  return val.hour * 3600 + val.minute * 60 + val.second;
}
