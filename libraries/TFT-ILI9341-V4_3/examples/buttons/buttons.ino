//===============================================
#include "TFT9341.h"
 
void setup() {	
  lcd.begin();
  lcd.setBackground(BLACK);
  lcd.clrscr();
  lcd.setColor (RED);

  lcd.fillRoundRect (10,10,80,40,6);
  lcd.fillRoundRect (100,100,80,40,10,YELLOW);
}

void loop() {
}
//===============================================
