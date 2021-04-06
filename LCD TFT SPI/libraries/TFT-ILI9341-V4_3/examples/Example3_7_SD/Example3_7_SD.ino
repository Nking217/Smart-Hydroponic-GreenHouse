//===============================================
//                   Example3_7
//===============================================
#include <SdFat.h>
#include <SPI.h>

#include "TFT9341.h"
#include "bitmap.h"
SdFat sd;
#define SD_CS 4

void setup() {
  
  
  Serial.begin(9600);
  lcd.begin();
  lcd.clrscr();
  lcd.print("Initializing SD card...");
  if (!sd.begin(SD_CS, SPI_QUARTER_SPEED)) 
        lcd.println("failed!");
   else
         lcd.println("OK!");
     delay(1000);
     lcd.clrscr();
    
    
}//End of setup function 


void loop() {


   
     bmp::draw("logo1.bmp", 0, 0);
     delay(1000);
      bmp::draw("view1.bmp", 0, 0);
     delay(1000);
      bmp::draw("view2.bmp", 0, 0);
     delay(1000);
      bmp::draw("view3.bmp", 0, 0);
     delay(1000);
      bmp::draw("view4.bmp", 0, 0);
     delay(1000);
      bmp::draw("view5.bmp", 0, 0);
     delay(1000);
      bmp::draw("view6.bmp", 0, 0);
     delay(1000); 
      bmp::draw("view7.bmp", 0, 0);
     delay(1000);
     bmp::draw("view8.bmp", 0, 0);
     delay(1000);
    }//End of loop function 

//===============================================
//         End Of File "Example3_7"        
//===============================================

