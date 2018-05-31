#include "U8glib.h"

const byte backlight = 11;
 
//Delcare the display and assign the pins
 
U8GLIB_PCD8544 u8g(8, 4, 7, 5, 6);  // CLK=8, DIN=4, CE=7, DC=5, RST=6

typedef struct modeTree{
  byte modeID;
  byte* prevMode;
};
 
void setup() {
  buildMenuStruct();
  pinMode(backlight, OUTPUT);
  digitalWrite(backlight, HIGH);
}
 
void loop() { 
 u8g.firstPage(); 
  do {
    drawBegin();
  } while( u8g.nextPage() ); 
}

//initializes the 
void buildMenuStruct(){
  
}

/*
void draw(){
  switch(mode){
    case 0: drawBegin();
            break;
    case 1: drawMenu();
            break;
    case 2: drawBegin();
            break;
    case 3: drawBegin();
            break;
    case 4: drawBegin();
            break;
    case 5: drawBegin();
            break;                               
  }
}
*/

void draw(){
  
}

void drawBegin() {
    u8g.setFont(u8g_font_profont11);
    u8g.drawStr(12, 10, F("Welcome to"));
    u8g.drawStr(28, 22, F("AVROS"));
    u8g.drawStr(1, 45, F("2018 Alex Lee"));  // put string of display at position X, Y
}

void drawMenu(){
}


