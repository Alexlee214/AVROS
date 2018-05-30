#include "U8glib.h"

const byte backlight = 11;
 
//Delcare the display and assign the pins
 
U8GLIB_PCD8544 u8g(8, 4, 7, 5, 6);  // CLK=8, DIN=4, CE=7, DC=5, RST=6
 
//char* myText = "Alex Lee";
 
void setup() {
  u8g.setFont(u8g_font_profont11);
  analogWrite(backlight, 255);
  drawBegin();
  
}
 
void loop() { 
 u8g.firstPage(); 
  do {
    drawBegin();
  } while( u8g.nextPage() ); 
  
}

void drawBegin() {
  u8g.drawStr(20, 10, "Alex Lee");  // put string of display at position X, Y
  u8g.drawStr(28, 18, "Says");
  u8g.drawStr(25,26, "Hello");
  delay(1000);
  
}

void drawMenu(){
  
}


