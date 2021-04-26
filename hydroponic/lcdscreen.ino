//////////////////
/// LCD SCREEN ///
//////////////////
#include "ili9488.h"
void lcdInit(){
  lcd.Init_LCD();
  lcd.Fill_Screen(BLACK);
}

void lcdTest(){
  lcdShowHomeScreen();
 // lcdShowHomeScreenStatus();
}

void lcdNotifyError(String error){
  
}

void lcdShowState(){ //All the lcd screen stuff (idk).
  
}

void lcdShowHomeScreen(){
  
  if (_Lcd_Status == LCD_PAGE_HOME){
    return;
  }
  _Lcd_Status = LCD_PAGE_HOME;
  lcd.Set_Text_Size(4);
  lcd.Print_String("Home Screen", 30, 4);
  lcd.Draw_Fast_HLine(25,35,265); //make a line on the screen
  /*lcd.Set_Text_Size(2);
  lcd.gotoxy(5,40);
  lcd.print("Status");
  lcd.drawRect(5,60,150,180); //Status show area
  lcd.Set_Text_Size(2);
  lcd.gotoxy(10,65);
  lcd.print("NFosset:");
  lcd.gotoxy(10,84);
  lcd.print("OFosset:");
  lcd.gotoxy(10,103);
  lcd.print("Pump:");
  lcd.gotoxy(10,122);
  lcd.print("Drainage:");
  lcd.gotoxy(10,141);
  lcd.print("Battery:");
  lcd.gotoxy(10,160);
  lcd.print("Water:");
  */
}


/*
void lcdShowHomeScreenStatus(){
  lcdWriteStatus(100, 65, _NfossetStatus.Short());
  lcdWriteStatus(100, 84, ofossetStatus_Short());
  lcdWriteStatus(100, 103, pumpStatus_Short());
  lcdWriteStatus(100, 122, drainageStatus_Short());
  lcdWriteStatus(100, 141, batteryStatus_Short());
}


void lcdWriteStatus(int x, int y, StatusResult res)
{
  lcdChangeColor(res.Priorty);
  print(x, y, res.StatusText);
}
void lcdChangeColor(int priorty)
{
  switch (priorty)
  {
    case STATUS_OK:
      lcd.setColor(STATUS_OK_COLOR); 
      break;
    case STATUS_WARNING:
      lcd.setColor(STATUS_WARNING_COLOR); 
      break;
    case STATUS_ERROR:
      lcd.setColor(STATUS_ERROR_COLOR);
      break;
    default:
      lcd.setColor(STATUS_NORMAL_COLOR);
      break;
  }
}
void lcdShowStatusScreen(){
  
}

void lcdShowSetupScreen(){
  
}

void print(int x, int y, String text)
{
  lcd.gotoxy(x, y);
  lcd.print((char*)text.c_str());
}
*/
