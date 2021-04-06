#include "logger.h"

#include <SdFat.h>

#define SD_CS 4 //cs pin

void setup() {
  SdFat sd;           //create sd instance.
 
  Serial.begin(9600); //Initiate serial communication at a baud rate of 9600.
  
  if(!sd.begin(SD_CS, SPI_QUARTER_SPEED)) { //begin serial communication with sd card at quarter spi speed.
    Serial.println(F("SD card initialization failed"));
    return;
  }

  file_io::Logger logger; //create logger instance.
 
  if(!logger.createLog("logger.txt")) { //check if log was created with the supplied name.
    Serial.println(F("Logger already exists, truncating logger"));
    if(!logger.openLog("logger.txt", true)) { //we open the log and truncate it to 0 length.
      Serial.println(F("Unable to open logger.txt"));
    }
  }

  if(logger.isOpen()) { //check if a log is currently open.
    if(!logger.push(123.123)) //push a value into log
      Serial.println(F("push(123.123) failed"));
      
    if(!logger.push(12.123)) //push a value into log
      Serial.println(F("push(12.123) failed"));
      
    if(!logger.push(0.0)) //push a value into log
      Serial.println(F("push(0.0) failed"));
      
    if(!logger.push(32.11)) //push a value into log
      Serial.println(F("push(32.11) failed"));
    
    if(!logger.pop()) //pop last value added (32.11)
      Serial.println(F("pop has failed"));

    if(!logger.setElementAt(2, 1.123)) //overwrite 0.0 to 1.123456
      Serial.println(F("setElementAt(2, 1.123) failed"));

    float result;
    if(!logger.getElementAt(2, result)) //read value at index 2 into result
      Serial.println(F("getElementAt(2, result) failed"));
    else {
      Serial.print(F("element at index 2 is: "));
      Serial.println(result, 3); //print the result with 3 decimal places   
    }

    Serial.print(F("size: "));
    Serial.println(logger.size()); //print log file element count
    if(logger.empty()) //print whether log has more than 0 elements
      Serial.println(F("log is empty"));
    else 
      Serial.println(F("log not empty"));
  }

  logger.closeLog(); //Logger's destructor will automatically call this function but we might 
                     //need to call it before the destructor is called to reopen other logs.
}

void loop() {
 
}
