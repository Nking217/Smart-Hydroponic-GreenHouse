//===============================================
//                   Example4_6
//===============================================
#include "TFT9341.h"
byte screen=1,ch=1;
word x,y;
void setup() {	
  lcd.begin();
}
void loop() {
 
     switch (screen){
      case 1: screen1(); break;
      case 2: screen2(); break;
      case 3: screen3(); break;
    }
   
}//End of loop function 
//===============================================
//         End Of File "Example4_5"        
//===============================================

