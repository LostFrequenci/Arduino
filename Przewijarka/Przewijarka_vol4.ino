#include <LiquidCrystal.h> //Dołączenie bilbioteki dla ekranu LCD 

// Przypisanie DO/DI dla ekranu LCD
const int rs = 12, en = 11, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Deklaracja zmiennych 
long temp, counter = 0; 
const int zeroButton = 8; 
int buttonState = 0;
float pomiar = 0;
    
void setup() {
    
  pinMode(zeroButton, INPUT); // internal pullup input pin 8
  pinMode(2, INPUT_PULLUP); // internal pullup input pin 2 
  pinMode(3, INPUT_PULLUP); // internal pullup input pin 3
  
  //Setting up interrupt
  //A rising pulse from encodenren activated ai0(). AttachInterrupt 0 is DigitalPin nr 2 on moust Arduino.
  attachInterrupt(0, ai0, RISING);
   
  //B rising pulse from encodenren activated ai1(). AttachInterrupt 1 is DigitalPin nr 3 on moust Arduino.
  attachInterrupt(1, ai1, RISING);

  lcd.begin(16, 2); //Deklaracja typu wyświetlacza LCD
  lcd.setCursor(0, 0); //Ustawienie kursora
  lcd.print("Pomiar obrzeza"); //Wyświetlenie tekstu

  }
   
  void loop() {

  pomiar = (float)((float)0.158 * (float)((float)counter/800));

  lcd.setCursor(0, 0); //Ustawienie kursora
  lcd.print("Pomiar obrzeza"); //Wyświetlenie tekstu
  lcd.setCursor(1,1);
  lcd.print(pomiar,2); //Wyświetlenie tekstu
  lcd.setCursor(8,1);
  lcd.print("MB");
  
  buttonState = digitalRead(zeroButton);
  if (buttonState == HIGH) { 
    counter = 0;
    lcd.clear();
  }

  // Send the value of counter
  if( counter != temp ){ temp = counter; }
  
  }
   
  void ai0() {
  // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
  // Check pin 3 to determine the direction
    if(digitalRead(3)==LOW) {
      counter++;
    }else{
      counter--;
    }
  }
   
  void ai1() {
  // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
  // Check with pin 2 to determine the direction
    if(digitalRead(2)==LOW) {
      counter--;
    }else{
      counter++;
    }
  }
