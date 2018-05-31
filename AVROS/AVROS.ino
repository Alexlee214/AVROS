#include "U8glib.h"
#include <StackArray.h>
#include <StackList.h>

const byte backlight = 11;
const byte joyX = A0;
const byte joyY = A1;
const byte joyClick = 3;

short joyXCenter = 510;
short joyYCenter = 510;


//general purpose registers used in all modes
//reset all to 0 after mode change
byte registers[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

byte numRegisters = 10;

//for storing modes, use whickever one uses least memory
//StackArray <byte> prevModes;
StackList <byte> prevModes;

volatile bool buttonPressed = false;

//invariant: mode branches are always less than parent mode quantitatively
//mode 0 : startup screen
//mode 1 : main screen
//mode 2 : functions
//mode 3 : startup screen
//mode 4 : startup screen
//mode 5 : startup screen
byte curMode = 0;

//Delcare the display and assign the pins
U8GLIB_PCD8544 u8g(8, 4, 7, 5, 6);  // CLK=8, DIN=4, CE=7, DC=5, RST=6


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------- Function declarition
void setupJoy();
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------- setup
void setup() {
  //interrupt for joystick button on pin 3
  attachInterrupt(1, joyInterrupt, FALLING);
  setupJoy();
  pinMode(backlight, OUTPUT);
  digitalWrite(backlight, HIGH);
  Serial.begin(9600);
  u8g.setFont(u8g_font_profont11);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------


 //--------------------------------------------------------------------------------------------------------------------------------------------------------------------loop
void loop() { 
 u8g.firstPage(); 
  do {
    draw();
  } while( u8g.nextPage() );

  modeAction(curMode);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------------------------------------------------------------------------modeAction
//determines what should happen in each mode
void modeAction(byte curMode){
  switch(curMode){
    case 0: radiusIncrement();
            break;        
    case 1: drawMain();
            break;
    case 2: drawFunctions();
            break;
    case 3: drawBegin();
            break;
    case 4: drawBegin();
            break;
    case 5: drawBegin();
            break;  
    default: drawBegin();
             break;                             
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------


void joyInterrupt(){
  buttonPressed = true;
  Serial.println(F("INTERRUPT"));
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------------incrementRadius
// used for increasing the radius in mode 1
//register 0 used to store radius
void radiusIncrement(){
  byte* myRadius = &registers[0];
  //increment the value at register 0 if still starting up

  if(*myRadius == 0){
    delay(1500);
    *myRadius = (*myRadius) + 1;
  }
  else if((*myRadius) < 45){
    *myRadius = (*myRadius) + 1;
    delay(100);
  }else{
    modeChange(1);
  }
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------draw
//determines which draw function to call based on the current mode we are in
void draw(){
  switch(curMode){
    case 0: drawBegin();
            break;        
    case 1: drawMain();
            break;
    case 2: drawFunctions();
            break;
    case 3: drawBegin();
            break;
    case 4: drawBegin();
            break;
    case 5: drawBegin();
            break;  
    default: drawBegin();
             break;                             
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------------------------------------------------------------------------drawBegin
//draws the welcome/startup screen
void drawBegin() {
    byte* myRadius = &registers[0];
    //u8g.setFont(u8g_font_courR08);
    //u8g.setFont(u8g_font_6x10);
    u8g.drawStr(12, 13, F("Welcome to"));
    u8g.drawStr(28, 26, F("AVROS"));
    u8g.drawStr(1, 45, F("2018 Alex Lee"));  // put string of display at position X, Y
    u8g.drawDisc(42, 22, *myRadius, U8G_DRAW_ALL);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------


void drawMain(){
  u8g.drawStr(12, 10, F("MAIN"));
}

void drawFunctions(){
  
}























//stores the current mode in the buffer and advance to the next mode
void modeChange(byte nextMode){
  if(nextMode > curMode) prevModes.push(curMode);
  curMode = nextMode;
  registerRestore();
}

void prevMode(){
  curMode = prevModes.pop();
  registerRestore();
}


void registerRestore(){
  for(byte countReg = 0; countReg < numRegisters; countReg++)
  registers[countReg] = 0;
}

void setupJoy(){
  pinMode(joyX, INPUT);
  pinMode(joyY, INPUT);
  pinMode(joyClick, INPUT_PULLUP);

  //calibrate the centre positions of the joystick
  short sumJoy = 0;
  for(byte countJoy = 0; countJoy < 10; countJoy++) sumJoy = sumJoy + analogRead(joyX);
  joyXCenter = sumJoy / 10;
  sumJoy = 0;
  for(byte countJoy = 0; countJoy < 10; countJoy++) sumJoy = sumJoy + analogRead(joyY);
  joyYCenter = sumJoy / 10;
}
