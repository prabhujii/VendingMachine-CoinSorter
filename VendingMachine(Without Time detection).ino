#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;
const int pinIRd = 11;
const int pinIRa = A0;
const int pinLED = 9;

// Digital Pins for infrared sensors
const int quarterPin = 11;
const int looniePin = 12;
//const int tooniePin = 13;

int IRvalueA = 0;
int IRvalueD = 0;

//Digital Pin value for the three sensors
int quarterPinVal = 0;
int looniePinVal = 0;
int tooniePinVal = 0;
// Current amount of money the user inserted 
int currentsum = 0;

double itemprice =  0;  // The price of the item user selected

//Use time to check how long user didn't respond.
unsigned long startTime;
unsigned long stopTime;


boolean pressed = false;
String items []  = {"A1", "A2"};
double prices []  = {3.00,2.50};
String s;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

LiquidCrystal_I2C lcd(0x3F, 20, 4);  

//Welcome Screen
//This function is called at the beginning or when the user has inputed the '#' sign to go back to the welcome screen.
void welcome(){
  currentsum = 0;
  lcd.clear();
  lcd.print("Please enter");
  lcd.setCursor(0,1);
  lcd.print("your selection");
}

void payment()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  int index;
  boolean flag = 0;
  for (int i=0;i<2;i++)
  {
     if (s==items[i]) {index=i;flag=1;}
  }
  if (!flag) {lcd.print("Item not found");return;}
  lcd.print("$");
  lcd.print(prices[index]);
  itemprice=prices[index];
  
}

// Check if the user inserted enough coin for the item
void checkcoin()
{
  if (currentsum == itemprice && currentsum !=0)
  {
    // Turn servos
    welcome();
  }
}

void setup(){
  Serial.begin(9600);
  lcd.backlight();
  lcd.init(); 
  welcome();
  pinMode(pinIRd,INPUT);
  pinMode(pinIRa,INPUT);
  pinMode(quarterPin,INPUT);
  pinMode(looniePin,INPUT);
  
}

void loop(){
 
 IRvalueA = analogRead(pinIRa);
 IRvalueD = digitalRead(pinIRd);
 
 quarterPinVal=digitalRead(quarterPin);
 looniePinVal=digitalRead(looniePin);
 //tooniePinVal=digitalRead(tooniePin);
  
 if (looniePinVal == 0)
 {
   Serial.println("Loonie went through!");
   currentsum+=1;
   delay(200);
 }
 if (quarterPinVal == 0)
 {
   Serial.println("Quarter went through!");
   currentsum+=0.25;
   delay(200);
 }


  
  checkcoin();
  char customKey = customKeypad.getKey();
  if (customKey){
    if (customKey=='#'){
      welcome();
    }
    else
    {
      
      if (s.length()<2)
      {
        s=s+customKey;
        startTime = millis();
        lcd.clear();
        lcd.setCursor(0, 0); 
        lcd.print(s);

      }
      if (s.length()==2){

        payment();
        s="";

      }
    }
   }
}
