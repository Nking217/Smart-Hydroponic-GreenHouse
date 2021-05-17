//////////////////
/// LCD SCREEN ///
//////////////////
#include "ili9488.h"
void lcdInit(){
  lcd.Init_LCD();
  lcd.Fill_Screen(WHITE);
}

void lcdTest(){
  lcdShowHomeScreen();
  lcdShowHomeScreenStatus();
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
  lcd.Set_Rotation(1);
  lcd.Set_Text_Size(4);
  lcd.Set_Text_colour(BLACK);
  lcd.Set_Text_Back_colour(BLACK);
  lcd.Print_String("Home Screen", 105, 4);
  lcd.Set_Draw_color(BLACK);
  lcd.Draw_Fast_HLine(105,35,265); //make a line on the screen
  lcd.Set_Text_Size(2);
  lcd.Print_String("Status", 5, 45);
  lcd.Draw_Rectangle(5,60,230,315);
  lcd.Print_String("Nfosset:", 10, 65);
  lcd.Print_String("Ofosset:", 10, 85);
  lcd.Print_String("Pump:", 10, 105);
  lcd.Print_String("Drainage:", 10, 125);
  lcd.Print_String("Battery:", 10, 145);
  lcd.Print_String("Water:", 10, 165);
}


void lcdShowHomeScreenStatus(){
  lcdWriteStatus(105, 65, _NfossetStatus.Short);
  lcdWriteStatus(105, 85, _OfossetStatus.Short);
  //lcdWriteStatus(100, 105, pumpStatus_Short());
 // lcdWriteStatus(100, 125, drainageStatus_Short());
 // lcdWriteStatus(100, 145, batteryStatus_Short());
}


void lcdWriteStatus(int x, int y, StatusResult res){
  lcd.Set_Text_Back_colour(WHITE);
  Serial.println(res.StatusText);
  lcdChangeColor(res.Status);
  print(x, y, res.StatusText);
}


void lcdChangeColor(int status){
  switch (status)
  {
    case STATUS_OK:
      lcd.Set_Text_colour(STATUS_OK_COLOR);
      Serial.print("Ok"); 
      break;
    case STATUS_WARNING:
      lcd.Set_Text_colour(STATUS_WARNING_COLOR);
      Serial.print("Warning"); 
      break;
    case STATUS_ERROR:
      lcd.Set_Text_colour(STATUS_ERROR_COLOR);
      Serial.print("Error");
      break;
    default:
      lcd.Set_Text_colour(STATUS_NORMAL_COLOR);
      Serial.print("Normal");
      break;
  }
}
void lcdShowStatusScreen(){
  
}

void lcdShowSetupScreen(){
  
}

void print(int x, int y, String text){
  lcd.Print_String(text, x, y); 
}
