#include <LiquidCrystal.h>
#include <Servo.h>
#include <Keypad.h>

/************************************
 * Arduino Keypad Door Lock.
 * 
 * Code by Victor (Borui) Niu
 * bniu2@illinois.edu
 * www.github.com/Lathie
 * 
 * 
 ************************************/

const byte ROWS = 4; // Four rows
const byte COLS = 3; // Three columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'#','0','*'}
};

//Define Victor's Variables :D
int icount;
char input[4];
char MASTERKEY[4] = {'8','8','8','8'};
bool doorLocked = false; 

//Define Servo stuff
Servo myServo;
int pos = 0;
const int lockPos = 280; //PLACEHOLDER UNTIL ANGLE IS DETERMINED
const int unlockPos = 70; //PLACEHOLDER UNTIL ANGLE IS DETERMINED
//int backForthMovement = 150;

//Define STATES
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);//FIX THIS SHIT
const char LCDStart[16]={'E','N','T','E','R',' ','P','I','N',' ',' ',' ',' ',' ',' ',' '};
const char LCDWelcome[16] = {'W','E','L','C','O','M','E','!',' ',' ',' ',' ',' ',' ',' ',' '};
const char LCDGoodbye[16] = {'G','O','O','D','B','Y','E','!',' ',' ',' ',' ',' ',' ',' ',' '};
const char LCDLocked[16] = {'L','O','C','K','E','D','!',' ',' ',' ',' ',' ',' ',' ',' ',' '};
const char LCDUnlocked[16] = {'U','N','L','O','C','K','E','D','!',' ',' ',' ',' ',' ',' ',' '};
const char LCDFuckOff[16] = {'G','O',' ','A','W','A','Y','!',' ',' ',' ',' ',' ',' ',' ',' '};
const char LCDAdminMode[16] = {'A','D','M','I','N',' ','M','O','D','E',' ',' ',' ',' ',' ',' '};
const char LCDPINAdd[16] = {'P','I','N',' ','A','D','D','E','D',' ',' ',' ',' ',' ',' ',' '};
const char LCDPINDelete[16] = {'P','I','N',' ','D','E','L','E','T','E','D',' ',' ',' ',' ',' '};

// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 9, 8, 7, 6 };
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { 12, 11, 10 }; 
// Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void reset(){

  //reset LCD screen
  lcd.clear();
  lcd.print(LCDStart);
  icount = 0;
 
}

void servoLock(){
  for (pos = unlockPos; pos <= lockPos; pos+=1){
    myServo.write(pos); 
    delay(15); //Fine tune the servos
  }
}

void servoUnlock(){
  for (pos = lockPos; pos >= unlockPos; pos-=1){
    myServo.write(pos); 
    delay(15); //Fine tune the servos
  } 
}

void correctCode(){

  //update LCD screen
  if (!doorLocked){
    lcd.clear();
    lcd.print(LCDGoodbye);
    lcd.setCursor(0,1);
    lcd.print(LCDLocked);
    servoLock();
    doorLocked = true;
  }
  else {
    lcd.clear();
    lcd.print(LCDWelcome);
    lcd.setCursor(0,1);
    lcd.print(LCDUnlocked);
    servoUnlock();
    doorLocked = false;
  }
  reset();
}

void goAway(){
   lcd.clear();
   lcd.print(LCDFuckOff);
   reset();
}

//compare 2 character arrays.
bool compare4(char a[4], char b[4]){ 
  for (int i = 0; i < 4; i++){
    if (a[i] != b[i]){
      return false;
    }
  }
  return true;
    
}

void setup() {
  icount = 0;
  myServo.attach(9); //Fix the pins
  lcd.begin(16, 2); //columns, row
  lcd.print(LCDStart);
  lcd.setCursor(icount, 1);

}

//froot loop
void loop() {
  lcd.setCursor(icount, 1);
  char key = kpd.getKey();
  if (icount < 4) {
    if(key)  // Check for a valid key.
    {  
      input[icount] = key;
      lcd.print(key);
      icount += 1;
    } 
  }
  else {
    if (compare4(input, MASTERKEY)){//later force admin mode
      correctCode();
    }
    else{
      goAway();
    }
  }
}


