//===============================================
//                  "printCnt"
//===============================================
  void printCnt(byte posX,byte cnt,byte col){
      if(!col)
           lcd.setColor(BLUE);
       else
      lcd.setColor(BLACK);   
      lcd.gotoxy(posX,90);
      lcd.print(cnt/100);
      lcd.print(cnt/10%10);
      lcd.print(cnt%10);

}//End of printTimeTft function
//===============================================
//            End of file "printCnt"
//===============================================     

