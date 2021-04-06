//===============================================
//                printScreen
//===============================================

#include "bitmap.h"

void printScreen(){
   lcd.clrscr();
   lcd.setFont(2);
   lcd.setColor(BLUE);   
   lcd.gotoxy(20,10);
   lcd.print("====== Example4_5 ======");
   lcd.setFont(3);
   lcd.gotoxy(22,50);
   lcd.print("cnt1");
   lcd.gotoxy(127,50);
   lcd.print("cnt2");
   lcd.gotoxy(232,50);
   lcd.print("cnt3");
   lcd.drawRect(22 ,80, 70, 40,MAGENTA);
   lcd.drawRect(127,80, 70, 40,MAGENTA);
   lcd.drawRect(232,80, 70, 40,MAGENTA);
 
   bmp::draw("ICON1.bmp",  10, 170);
   bmp::draw("ICON2.bmp",  90, 170);
   bmp::draw("ICON3.bmp", 170, 170);
   bmp::draw("ICON4.bmp", 250, 170);
  //lcd.setColor(BLACK); 
}//End of printScr function 
//===============================================
//         End Of File "printScreen"        
//===============================================

