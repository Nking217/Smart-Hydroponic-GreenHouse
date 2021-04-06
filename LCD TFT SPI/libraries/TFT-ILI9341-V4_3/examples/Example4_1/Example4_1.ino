//===============================================
//                   Example4_1
//===============================================
#include "TFT9341.h"
unsigned int x,y;
void setup() {  
  lcd.begin();
  lcd.clrscr();
  lcd.setColor (BLUE);
  lcd.setFont(7);
  lcd.gotoxy(60,50);
  lcd.print("Touch");
  lcd.setFont(2);
  lcd.setColor (GREEN);
  }//End of setup function
void loop() {
  while(digitalRead(2)==1);
  lcdtouch.readxy();
  x = lcdtouch.readx();
  y = lcdtouch.ready();
  lcd.gotoxy(50,140);
  lcd.print("                     ");
  lcd.gotoxy(50,140);
  lcd.print ("x = ");  
  lcd.print(x);
  lcd.print ("     y = ");  
  lcd.print(y);
}//End of loop function
//===============================================
//         End Of File " Example4_1"        
//===============================================

