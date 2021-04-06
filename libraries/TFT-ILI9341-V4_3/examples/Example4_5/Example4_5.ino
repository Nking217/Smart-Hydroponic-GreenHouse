//===============================================
//                   Example4_5
//===============================================

#include "TFT9341.h"

#include <SdFat.h>
#define speakerPin A1
#define greenLed 3
#define redLed 5
#define bluenLed 6
unsigned int x,y;
 byte menu=0,sel=0,counter1,counter2,counter3;
int toneArr[4]={523, 698, 787,887};
#define SD_CS 4
void setup() {
  SdFat sd;
  lcd.begin();


    
  lcd.clrscr();
  lcd.print("Initializing SD card...");
    if (!sd.begin(SD_CS, SPI_QUARTER_SPEED)) 
         lcd.println("failed!");
   else{
       lcd.println("OK!");
       delay(1000);
       lcd.clrscr();
       printScreen();
      }
 }//End of setup function 
void loop() {
     printCnt(30,counter1,1);
     printCnt(135,counter2,1);
     printCnt(240,counter3,1);
     switch(menu){
     case 0:  funCnt1();
              break;
     case 1:  funCnt2();
              break;
     case 2:  funCnt3();             
              break;
      }//End switch
    // }//End if
    }//End of loop function 
//===============================================
//         End Of File "Example4_5"        
//===============================================


