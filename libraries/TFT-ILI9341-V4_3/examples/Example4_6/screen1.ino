//===============================================
//                  funCnt3
//===============================================
void screen1(){
  lcd.clrscr (WHITE);
  lcd.fillRoundRect (20,20,70,40,5,RED);
  lcd.setColor(WHITE,RED);
  lcd.setFont (2);
  lcd.gotoxy (25,30);
  lcd.print ("PRESS");
  do{
    readTouch();
    if ((x>20) && (x<90) && (y>20) && (y<60)){
       screen=2;
    }
  }while(screen==1);
}//End of screen1 function 
//===============================================
//         End Of File " funCnt1"        
//===============================================

