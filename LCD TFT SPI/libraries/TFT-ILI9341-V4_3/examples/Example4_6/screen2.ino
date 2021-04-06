//===============================================
//                  screen2
//===============================================
void screen2(){
 ch=0;
  lcd.clrscr (BLUE);
  lcd.fillRoundRect (20,20,70,40,5,YELLOW);
  lcd.setColor(BLACK,YELLOW);
  lcd.setFont (2);
  lcd.gotoxy (25,30);
  lcd.print ("NEXT");
    do{
      readTouch();
      if ((x>20) && (x<90) && (y>20) && (y<60)){
          screen=3;
      }
  }while(screen==2);
}//End of screen2 function 
//===============================================
//         End Of File "screen2"        
//===============================================

