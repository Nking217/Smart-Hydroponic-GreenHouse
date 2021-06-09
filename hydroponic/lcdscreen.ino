//////////////////
/// LCD SCREEN ///
//////////////////
#include "ili9488.h"
#include "ili9488_touch.h"
void lcdInit(){
  lcd.Init_LCD();
  lcdT.TP_Init();
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

/////////////////
//-Home Screen-//
/////////////////
void lcdShowHomeScreen(){
  if (_Lcd_Status == LCD_PAGE_HOME){
    return;
  }
  _Lcd_Status = LCD_PAGE_HOME;
  lcd.Fill_Screen(WHITE);
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
  lcd.Print_String("Temprature:", 10, 185);
  lcd.Print_String("Humidity:", 10, 205);
  lcd.Print_String("Turbidity:", 10, 225);
  lcd.Print_String("WI-FI:", 10, 245);
  lcd.Print_String("Server:", 10, 265);
  lcd.Print_String("Light:", 10, 285);
  //Buttons
  lcd.Set_Text_Size(4);
  lcd.Draw_Rectangle(240,60,470,130);
  lcd.Print_String("Status", 280,83);
  
  lcd.Draw_Rectangle(240,140,470,210);
  lcd.Print_String("Config", 280,160);
  
  lcd.Draw_Rectangle(240,220,470,290);
  lcd.Print_String("About", 280,240);
  lcd.Set_Text_Size(2);
}

void lcdShowHomeScreenStatus(){ //Status text blinking fix this TO:DO
  lcd.Print_String("       ", 105, 65);
  lcdWriteStatus(105, 65, _NfossetStatus.Short);
  lcd.Print_String("       ", 105, 85);
  lcdWriteStatus(105, 85, _OfossetStatus.Short);
  lcd.Print_String("       ", 75, 105);
  lcdWriteStatus(75, 105, _PumpStatus.Short);
  lcd.Print_String("       ", 120, 125);
  lcdWriteStatus(120, 125, _DrainageStatus.Short);
  lcd.Print_String("       ", 105, 145);
  lcdWriteStatus(105, 145, _BatteryStatus.Short);
  lcd.Print_String("       ", 105, 165);
  lcdWriteStatus(105, 165, _WaterHightStatus.Short);
  lcd.Print_String("       ", 140, 195);
  lcdWriteStatus(140, 185, _TemperatureStatus.Short);
  lcd.Print_String("       ", 115, 205);
  lcdWriteStatus(115, 205, _HumidityStatus.Short);
  lcd.Print_String("       ", 105, 225);
  //lcdWriteStatus(105, 225, ); Convert turbidity to statusresult (not working right now)  
}

void lcdHandleTouch(){
  if(lcdT.TP_Scan()){
    int x = lcdT.getX();
    int y = lcdT.getY();
    /// Buttons on the home screen ///
    if(_Lcd_Status == LCD_PAGE_HOME){ 
      if(x >= 240 && x <= 470 && y >= 60 && y <= 130){ //Status Screen button 240,60,470,130
        //Serial.println("Status");
        lcdShowStatusScreen();
        _Lcd_Status = LCD_PAGE_STATUS;
      }
      if(x >= 240 && x <= 470 && y >= 140 && y <= 210){ //Configuration Screen Button  240,140,470,210
        //Serial.println("Config");
        lcdShowConfigScreen();
        _Lcd_Status = LCD_PAGE_CONFIG;
      }
      if(x >= 240 && x <= 470 && y >= 220 && y <= 290){ //About Screen Button 240,220,470,290
        //Serial.println("About");
        lcdShowAboutScreen();
        _Lcd_Status = LCD_PAGE_ABOUT;
      }
    }

    
    /// Buttons on the status screen ///
    if(_Lcd_Status == LCD_PAGE_STATUS){
      //*back to home and back to status buttons configuration
      if(_Lcd_Status_Subpage == STATUS_SUBPAGE_MAIN){
        if(x >= 4 && x <= 60 && y >= 5 && y <= 30){ //Back to home button
        //Serial.println("Back to home button");
        lcdShowHomeScreen();
        _Lcd_Status = LCD_PAGE_HOME;
        }
      }
      else{
        if(x >= 4 && x <= 60 && y >= 5 && y <= 30){ //Back to status button
        //Serial.println("Back to home button");
        lcdShowStatusScreen();
        _Lcd_Status = LCD_PAGE_STATUS;
        }
      }

      //*Full status for sensors buttons
      if(x >= 4 && x <= 60 && y >= 5 && y <= 30){ //Nfosset status button
        
      }
    }

    
    /// Buttons on configuration screen ///
    if(_Lcd_Status == LCD_PAGE_CONFIG){
      if(x >= 4 && x <= 60 && y >= 5 && y <= 30){ //Status Screen button 240,60,470,130
        //Serial.println("Back to home button");
        lcdShowHomeScreen();
        _Lcd_Status = LCD_PAGE_HOME;
      }
    }

    
    /// Buttons on about screen ///
    if(_Lcd_Status == LCD_PAGE_ABOUT){
      if(x >= 4 && x <= 60 && y >= 5 && y <= 30){ //Status Screen button 240,60,470,130
        //Serial.println("Back to home button");
        lcdShowHomeScreen();
        _Lcd_Status = LCD_PAGE_HOME;
      }
    }
  }
}



void lcdShowStatusScreen(){
  _Lcd_Status = LCD_PAGE_STATUS;
  lcd.Fill_Screen(WHITE);
  lcd.Set_Rotation(1);
  lcd.Set_Text_colour(BLACK);
  lcd.Set_Text_Back_colour(BLACK);
  //*Back to home button
  lcd.Set_Text_Size(2);
  lcd.Draw_Rectangle(4,5,60,30);
  lcd.Print_String("Home", 10,10);
  
  lcd.Set_Text_Size(4);
  lcd.Set_Text_colour(BLACK);
  lcd.Set_Text_Back_colour(BLACK);
  lcd.Print_String("Status Screen", 90, 4);
  lcd.Set_Draw_color(BLACK);
  lcd.Draw_Fast_HLine(90,35,310); //make a line on the screen
  lcd.Set_Text_Size(2);
  //Short status for now
  lcd.Print_String("Nfosset:", 10, 65);
  lcd.Print_String("Ofosset:", 10, 85);
  lcd.Print_String("Pump:", 10, 105);
  lcd.Print_String("Drainage:", 10, 125);
  lcd.Print_String("Battery:", 10, 145);
  lcd.Print_String("Water:", 10, 165);
  lcd.Print_String("Temprature:", 10, 185);
  lcd.Print_String("Humidity:", 10, 205);
  lcd.Print_String("Turbidity:", 10, 225);
  lcd.Print_String("WI-FI:", 10, 245);
  lcd.Print_String("Server:", 10, 265);
  lcd.Print_String("Light:", 10, 285);

  /// Status buttons on the status screen ///
  //*Nfosset Button
    /*
  lcd.Set_Text_Size(3);
  lcd.Set_Text_colour(BLACK);
  lcd.Set_Text_Back_colour(BLACK);
  lcd.Draw_Rectangle(10,45,180,85);
  lcd.Print_String("Nfosset", 15, 55);
       
  //*Ofosset button
  lcd.Set_Text_Size(3);
  lcd.Set_Text_colour(BLACK);
  lcd.Set_Text_Back_colour(BLACK);
  lcd.Draw_Rectangle(10,65,180,125);
  lcd.Print_String("Ofosset", 15, 75);

  //*Pump Button
  lcd.Set_Text_Size(3);
  lcd.Set_Text_colour(BLACK);
  lcd.Set_Text_Back_colour(BLACK);
  lcd.Draw_Rectangle(10,65,180,125);
  lcd.Print_String("Pump", 15, 75);

  //*Drainage Button
  lcd.Set_Text_Size(3);
  lcd.Set_Text_colour(BLACK);
  lcd.Set_Text_Back_colour(BLACK);
  lcd.Draw_Rectangle(10,65,180,125);
  lcd.Print_String("Drainage", 15, 75);
  
*/
  
}

void lcdShowStatusScreenStatus(){
  lcd.Set_Text_Size(2);
  lcd.Print_String("          ", 105, 65);
  lcdWriteStatus(105, 65, _NfossetStatus.Short);
  lcd.Print_String("          ", 105, 85);
  lcdWriteStatus(105, 85, _OfossetStatus.Short);
  lcd.Print_String("          ", 75, 105);
  lcdWriteStatus(75, 105, _PumpStatus.Short);
  lcd.Print_String("          ", 120, 125);
  lcdWriteStatus(120, 125, _DrainageStatus.Short);
  lcd.Print_String("          ", 105, 145);
  lcdWriteStatus(105, 145, _BatteryStatus.Short);
  lcd.Print_String("          ", 105, 165);
  lcdWriteStatus(105, 165, _WaterHightStatus.Short);
  lcd.Print_String("          ", 140, 195);
  lcdWriteStatus(140, 185, _TemperatureStatus.Short);
  lcd.Print_String("          ", 115, 205);
  lcdWriteStatus(115, 205, _HumidityStatus.Short);
  lcd.Print_String("          ", 105, 225);
}


void lcdShowConfigScreen(){
  _Lcd_Status = LCD_PAGE_STATUS;
  lcd.Fill_Screen(WHITE);
  lcd.Set_Text_Size(4);
  lcd.Set_Text_colour(BLACK);
  lcd.Set_Text_Back_colour(BLACK);
  lcd.Print_String("Configuration", 90, 4);
  lcd.Set_Draw_color(BLACK);
  lcd.Draw_Fast_HLine(90,35,310); //make a line on the screen
  lcd.Set_Text_Size(3);
}


void lcdShowAboutScreen(){
  _Lcd_Status = LCD_PAGE_STATUS;
  lcd.Fill_Screen(WHITE);
  lcd.Set_Text_Size(4);
  lcd.Set_Text_colour(BLACK);
  lcd.Set_Text_Back_colour(BLACK);
  lcd.Print_String("About", 90, 4);
  lcd.Set_Draw_color(BLACK);
  lcd.Draw_Fast_HLine(90,35,310); //make a line on the screen
  lcd.Set_Text_Size(3);
}






void lcdWriteStatus(int x, int y, StatusResult res){
  lcd.Set_Text_Back_colour(WHITE);
  //Serial.println(res.StatusText);
  lcdChangeColor(res.Status);
  print(x, y, res.StatusText);
}


void lcdChangeColor(int status){
  switch (status)
  {
    case STATUS_OK:
      lcd.Set_Text_colour(STATUS_OK_COLOR);
      //Serial.print("Ok"); 
      break;
    case STATUS_WARNING:
      lcd.Set_Text_colour(STATUS_WARNING_COLOR);
      //Serial.print("Warning"); 
      break;
    case STATUS_ERROR:
      lcd.Set_Text_colour(STATUS_ERROR_COLOR);
      //Serial.print("Error");
      break;
    default:
      lcd.Set_Text_colour(STATUS_NORMAL_COLOR);
      //Serial.print("Normal");
      break;
  }
}


void print(int x, int y, String text){
  lcd.Print_String(text, x, y); 
}
