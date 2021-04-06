//===============================================
//                 Example4_2
//===============================================
#include "TFT9341.h"

word x,y;
byte i,flag=0;
#define redLed 5
void setup() {  
lcd.begin();
for(i=0;i<2;i++){
    lcd.drawCircle( 160, 80, 40+i,BLACK);
  }
pinMode(redLed,OUTPUT);
lcd.fillRoundRect (113,140,90,60,5,CYAN);
lcd.gotoxy(142,160);
lcd.setFont(3);
lcd.setColor (BLACK,CYAN);
lcd.print("SW"); 
}//End of setup function
void loop() {
while (digitalRead(2)==1);
delay(20);
lcdtouch.readxy();
x = lcdtouch.readx();
y = lcdtouch.ready();
if ((x>113) && (x<113+90) && (y>140) && (y<140+60))
    flag=~flag;
if(flag){
   lcd.fillCircle( 160, 80, 39,BLUE);
   digitalWrite(redLed,1);
    delay(100);
}
else{
   lcd.fillCircle( 160, 80, 39,WHITE);
   digitalWrite(redLed,0);
   delay(100);
 }
}//End of loop function
//===============================================
//         End Of File " Example4_2"        
//===============================================

