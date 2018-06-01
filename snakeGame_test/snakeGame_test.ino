#include "U8glib.h"

const byte backlight = 11;
const byte joyX = A0;
const byte joyY = A1;
const byte joyClick = 3;
volatile bool buttonPressed = false;

U8GLIB_PCD8544 u8g(8, 4, 7, 5, 6);  // CLK=8, DIN=4, CE=7, DC=5, RST=6

typedef struct{
  byte xCoor;
  byte yCoor;
} point;

short joyXCenter = 510;
short joyYCenter = 510;

bool boolRegisters[5] = {false, false, false, false, false};

unsigned long timer = millis();
unsigned long lastTimer = 0;

byte snakeLength = 4;
//point snake[20] = {{42, 46}, {42, 45}, {42, 44}, {42, 43}};
point snake[20] = {{42, 43}, {42, 44}, {42, 45}, {42, 46}};

//0 is up, 1 is left, 2 is down, 3 is right
byte snakeDirection = 0;

void setup() {
  pinMode(backlight, OUTPUT);
  digitalWrite(backlight, HIGH);
  pinMode(joyClick, INPUT_PULLUP);
  //interrupt for joystick button on pin 3
  attachInterrupt(1, joyInterrupt, FALLING);
  // put your setup code here, to run once:
  u8g.setFont(u8g_font_profont11);
  setupJoy();
  Serial.begin(9600);
}



void loop() {
  u8g.firstPage(); 
  // put your main code here, to run repeatedly:
  do {
    draw();
  } while( u8g.nextPage() );

  modeAction();
  Serial.println(boolRegisters[0]);
  Serial.println(boolRegisters[1]);
  Serial.println(boolRegisters[2]);
  Serial.println("");
}


//uses boolRegister[0] as game start
//uses boolRegister[1] as game pause
//uses boolRegister[2] as game over
void modeAction(){
  bool* gameStart = &boolRegisters[0];
  bool* gamePause = &boolRegisters[1];
  bool* gameOver = &boolRegisters[2];
    
  if(buttonPressed == true && *gameStart == false){
    *gameStart = true;
    buttonPressed = false;
  } else if(buttonPressed == true && *gameStart == true){
    *gamePause = true;
    buttonPressed = false;
  } else if(*gameStart == true && *gamePause == false && *gameOver == false){
    if(analogRead(joyX) < 25 && snakeDirection != 3){
      snakeDirection = 1;
    }else if(analogRead(joyX) > 1000 && snakeDirection != 1){
      snakeDirection = 3;
    }else if(analogRead(joyY) < 25 && snakeDirection != 2){
      snakeDirection = 0;
    }else if(analogRead(joyY) > 1000 && snakeDirection != 0){
      snakeDirection = 2;
    }
    snakeMove(snakeDirection);
  }
}

void snakeMove(byte snakeDirection){
  for(byte countSnake = snakeLength - 1; countSnake > 0; countSnake--){
                snake[countSnake] = snake[countSnake - 1];
   }
   switch(snakeDirection){
    //up
    case 0: snake[0].yCoor = snake[0].yCoor - 1;
            break;
    //left        
    case 1: snake[0].xCoor = snake[0].xCoor - 1;
            break;
    //down        
    case 2: snake[0].yCoor = snake[0].yCoor + 1;
            break;
    //right        
    case 3: snake[0].xCoor = snake[0].xCoor + 1;
            break;
    }

    delay(250);
}


void draw() {
    bool* gameStart = &boolRegisters[0];
    bool* gamePause = &boolRegisters[1];
    bool* gameOver = &boolRegisters[2];
    
    u8g.drawFrame(0,0,83,48);
    //game hasn't started yet
    if(*gameStart == false){
      u8g.drawStr(3, 13, F("Push to start"));
      for(byte countSnake = 0; countSnake < snakeLength; countSnake++)
        u8g.drawPixel(snake[countSnake].xCoor, snake[countSnake].yCoor);
        
    //game started but not paused or game over    
    } else if((*gamePause) == false && (*gameOver) == false){
      for(byte countSnake = 0; countSnake < snakeLength; countSnake++)
        u8g.drawPixel(snake[countSnake].xCoor, snake[countSnake].yCoor);

    //game paused    
    } else if((*gamePause) == true){
     u8g.drawStr(5, 13, F("Game paused"));
     for(byte countSnake = 0; countSnake < snakeLength; countSnake++)
        u8g.drawPixel(snake[countSnake].xCoor, snake[countSnake].yCoor);
        
    //game over  
    } else if((*gameOver) == true){
      u8g.drawStr(8, 13, F("Game over"));
      for(byte countSnake = 0; countSnake < snakeLength; countSnake++)
        u8g.drawPixel(snake[countSnake].xCoor, snake[countSnake].yCoor);
        
    }
}



void joyInterrupt(){
  if(timer - lastTimer > 1000)
    buttonPressed = true;
  lastTimer = millis();  
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
