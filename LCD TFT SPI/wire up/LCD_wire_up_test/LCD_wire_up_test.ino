#include "TFT9341.h"

void setup() 
{
  //initialize the LCD screen
  lcd.begin();

  //place cursor at location column 0 and row 0
  lcd.gotoxy(0,0);

  //clear screen
  lcd.clrscr();

  //clear screen to specific color
  //possible values: BLACK, GRAY, BLUE, RED, GREEN, CYAN, MAGENTA, PURPLE, ORANGE, YELLOW, WHITE
  //lcd.clrscr(BLUE);
  
  //set font size
  lcd.setFont(2);

  //set the color of the text and the background
  //possible values: BLACK, GRAY, BLUE, RED, GREEN, CYAN, MAGENTA, PURPLE, ORANGE, YELLOW, WHITE
  //lcd.setcolor(WHITE);
  lcd.setColor(WHITE, BLUE);

  //print something on screen
  lcd.print("hello ");
  lcd.println("world");
  lcd.print(10, 16);
}

int counter = 0;
void loop() 
{
  lcd.setColor(RED, WHITE);
  lcd.gotoxy(0,50);
  lcd.print(counter);
  counter += 1;
  delay(500);
}
