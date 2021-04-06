
//===============================================
//                  screen2
//===============================================
void screen3(){
  lcd.clrscr (RED);
  lcd.fillRoundRect (20,20,70,40,5,CYAN);
  lcd.setColor(BLACK,CYAN);
  lcd.setFont (2);
  lcd.gotoxy (25,30);
  lcd.print ("OK");
    do{
      readTouch();
      if ((x>20) && (x<90) && (y>20) && (y<60)){
      screen=1;
     }
  }while(screen==3);
}//End of screen3 function 
//===============================================
//         End Of File "screen3"        
//===============================================

