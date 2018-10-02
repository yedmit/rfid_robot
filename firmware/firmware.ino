#include <TimerOne.h>
#include <EEPROM.h>
#include <PCD8544.h>
#include "setup.h"

byte keypadRowPins[KEYPAD_ROWS] = {30};
byte keypadColPins[KEYPAD_COLS] = {22, 24, 26, 28};
boolean keypadState[KEYPAD_COLS][KEYPAD_ROWS] = {false, false, false, false};

int colorRed[] = {255, 0, 0};
int colorGreen[] = {0, 255, 0};
int colorBlue[] = {0, 0, 255};
int colorWhite[] = {255, 255, 255};
int colorOff[] = {0, 0, 0};

//
unsigned long sequenceStep = 1000;
//
int currentSpeed = 150;
//
String lastTagString = "";
//
volatile byte buttonPressed = BUTTON_NONE;
//
PCD8544 lcd = PCD8544(45, 43, 41, 39, 37);
//
String tags[TAG_AMOUNT];
//
String tagCharMemo [TAG_AMOUNT] = {"S","F","B","L","R","P", "FL","FR","BL","BR"};
String tagStringMemo [TAG_AMOUNT] = {
  "Stop",
  "Forward",
  "Backward",
  "Left",
  "Right",
  "Repeat",
  "Fwd Left",
  "Fwd Right",
  "Bwd Left",
  "Bwd Right",
};
//
byte tagSequence [TAG_SEQUENCE];
int sequenceLength = 0;
//
boolean isFrontLimitSwitchActive = false;
boolean isRearLimitSwitchActive = false;
//


//////
void setup() {
  Serial.begin(9600);
  RFID.begin(9600);  
  BLUETOOTH.begin(9600);

  pinMode(BUTTON_TAG_PIN, INPUT_PULLUP);
  pinMode(BUTTON_SET_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PLAY_PIN, INPUT_PULLUP);
  pinMode(BUTTON_BLUETOOTH_PIN, INPUT_PULLUP);
  pinMode(BUTTON_MENU_PIN, INPUT_PULLUP);
    
  pinMode(FRONT_LIMIT_SWITCH, INPUT_PULLUP);
  pinMode(REAR_LIMIT_SWITCH, INPUT_PULLUP);
  
  pinMode(RFID_RESET_PIN, OUTPUT);
  digitalWrite(RFID_RESET_PIN, HIGH);
  
  pinMode(LEFT_MOTOR_PIN_A, OUTPUT); 
  pinMode(LEFT_MOTOR_PIN_B, OUTPUT); 
  pinMode(RIGHT_MOTOR_PIN_A, OUTPUT);   
  pinMode(RIGHT_MOTOR_PIN_B, OUTPUT);   
  pinMode(LEFT_MOTOR_PIN_ENABLE, OUTPUT);  
  pinMode(RIGHT_MOTOR_PIN_ENABLE, OUTPUT);  
  
  digitalWrite(LEFT_MOTOR_PIN_ENABLE, LOW);  
  digitalWrite(RIGHT_MOTOR_PIN_ENABLE, LOW); 
    
  lcd.init();
  lcd.setContrast(50);
  lcd.clear();
  
  Timer1.initialize(100000);
  Timer1.attachInterrupt(timerIsr);
 

for (int i = 0; i < TAG_SEQUENCE; i++){
    tagSequence[i] = 0; 
    }

for (int i = 0; i < (TAG_AMOUNT * TAG_LENGTH); i = i + TAG_LENGTH){
    tags[i/TAG_LENGTH] = readTagFromEeprom(EEPROM_START_ADDRESS + i);
    }

}//setup()


//////
void loop() {
   lcd.clear();
   lcd.setCursor(LCD_MAIN_MENU_MEMO); 
   lcd.print("hit button");
   lcd.setCursor(LCD_MAIN_MENU_TAG); 
   lcd.print("TAG adjastment");
   lcd.setCursor(LCD_MAIN_MENU_SET); 
   lcd.print("SET sequence");
   lcd.setCursor(LCD_MAIN_MENU_PLAY); 
   lcd.print("PLAY sequence");   
   lcd.setCursor(LCD_MAIN_MENU_BLUETOOTH); 
   lcd.print("BLUETOOTH mode"); 
   //
  switch (buttonPressed) {
    case BUTTON_TAG: {
        adjustTags();
        break;
      }
    case BUTTON_SET: {
        setSequence();
        break;
      }
    case BUTTON_PLAY: {
        playSequence();
        break;
      }
      case BUTTON_BLUETOOTH: {
        bluetoothMode();
        break;
      }
  }
 // 
  lcd.display();
  delay(BUTTON_PAUSE);
}//loop()
