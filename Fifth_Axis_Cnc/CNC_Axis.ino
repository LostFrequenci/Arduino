/*
CNC_Axis Controler

Program sterujący dodatkową osią urządzenia CNC.

Prawa autorskie : Dominik Odroń
E-mail : dominik.odron@gmail.com
*/

// Deklaracja sygnałów wyjściowych sterujących servomotorem 
// ---------------------------------------------
int pulsePlus = 30; // Biały
int pulseMinus = 31; // Żółty
int directionPlus = 32;  // Zielony
int directionMinus = 33; // Niebieski
int enablePlus = 34;  // Brązowy
int enableMinus = 35;  // Czerwony

// Deklaracja sygnałów wejściowych
// ---------------------------------------------
int cncSignal = 45;
int zeroSignal = 43;

// Deklaracja zmiennych programowych
// ---------------------------------------------
String tabelaRuchow[200];
int tabelaKatow[200];
int tabelaPowtorzen[200];
int predkoscObrotowa = 0;
int wyborProgramu = 0;
int wyborDalej = 0;
int cncStateSignal = 0;
int zeroStateSignal = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  // Initialize digital pins as an output.
  // -------------------------------------
  pinMode(pulsePlus, OUTPUT);
  pinMode(pulseMinus, OUTPUT);
  pinMode(directionPlus, OUTPUT);
  pinMode(directionMinus, OUTPUT);
  pinMode(enablePlus, OUTPUT);
  pinMode(enableMinus, OUTPUT);
  // Initialize digital pins as an input.
  // ------------------------------------
  pinMode(cncSignal, INPUT_PULLUP);
  pinMode(zeroSignal, INPUT_PULLUP);
  //start serial connection
  // ------------------------------------
  Serial.begin(9600);
  delay(20);
  digitalWrite(pulsePlus, LOW);
  digitalWrite(directionPlus, LOW);
  digitalWrite(enablePlus, LOW); 
  digitalWrite(pulseMinus, LOW);
  digitalWrite(directionMinus, LOW);
  digitalWrite(enableMinus, HIGH);   
}

// Pętla główna 
void loop() {
  int wyborProgramu = 0;
  cncStateSignal = digitalRead(cncSignal);
  zeroStateSignal = digitalRead(zeroSignal);
  
  Serial.println("WYBIERZ PROGRAM :");
  Serial.println("1. NOWY PROGRAM");
  Serial.println("2. POPRZEDNI PROGRAM");
  
  waitToContinue();
  wyborProgramu = readIntValue();
    
  switch(wyborProgramu) {
    case 1:
        goto newProgram;
      break;
    case 2:
        goto oldProgram;
      break;
  }

  // Część programu odpowiedzialna za stworzenie nowego programu sterującego
  newProgram:
  Serial.println("Nowy program");
  predkoscObrotowa = 0;
  wyborDalej = 0;
  for (int a=0; a<200; a++){
    tabelaRuchow[a] = "";
    tabelaKatow[a] = 0;
    tabelaPowtorzen[a] = 0;
  }
  
  Serial.println("Podaj predkosc obrotowa...");
  waitToContinue();
  predkoscObrotowa = readIntValue();
  Serial.print("Predkosc obrotowa: ");
  Serial.println(predkoscObrotowa);
  
  for (int a=0; a<200; a++){
    Serial.println("Podaj funkcję...");
    waitToContinue();
    tabelaRuchow[a] = readStringValue();
    Serial.println("Podana funkcja: " + tabelaRuchow[a]);

    Serial.println("Podaj kat obrotu...");
    waitToContinue();
    tabelaKatow[a] = readIntValue();
    Serial.print("Kat obrotu: ");
    Serial.println(tabelaKatow[a]);

    Serial.println("Podaj ilosc powtorzen funkcji...");
    waitToContinue();
    tabelaPowtorzen[a] = readIntValue();
    Serial.print("Ilosc powtorzen: ");
    Serial.println(tabelaPowtorzen[a]);

    if (tabelaRuchow[a] == "koniec") {
      a = 222;
    }
  }

  Serial.println("WYBIERZ DALSZY KROK:");
  Serial.println("1. DALEJ...");
  Serial.println("2. POWTORZ PISANIE PROGRAMU");
  
  waitToContinue();
  wyborDalej = readIntValue();

  switch(wyborDalej) {
    case 1:
        goto oldProgram;
      break;
    case 2:
        goto newProgram;
      break;
  }
  
  // Część programu odpowiedzialna za wykonanie sterowania
  oldProgram:

  Serial.println("Wykonywanie programu !!!");
  for (int a=0; a<200; a++) {

    if (tabelaRuchow[a] == "ruch plus kat"){
      turnRightStep(predkoscObrotowa, tabelaKatow[a], tabelaPowtorzen[a]);
    }
    if (tabelaRuchow[a] == "ruch minus kat"){
      turnLeftStep(predkoscObrotowa, tabelaKatow[a], tabelaPowtorzen[a]);
    }
    if (tabelaRuchow[a] == "ruch plus"){
      turnRight();
    }
    if (tabelaRuchow[a] == "ruch minus"){
      turnLeft();
    }
    if (tabelaRuchow[a] == "ruch zero"){
      turnZero();
    }
  }
  Serial.println("====================");
  Serial.println("PROGRAM WYKONANY !!!");
  Serial.println("====================");
}

void turnRightStep(int predkoscObrotowa, int katObrotu, int iloscPowtorzen) {
  for (int a=0; a<iloscPowtorzen; a++){
    cncStateSignal = digitalRead(cncSignal);
    while (cncStateSignal == LOW) {
      cncStateSignal = digitalRead(cncSignal);
    }
    digitalWrite(enableMinus, HIGH);
    delay(20);
    digitalWrite(directionMinus, HIGH);
    delay(20);
    
    long iloscObrotow = (long)predkoscObrotowa*katObrotu*2/360;
    Serial.print("Ruch plus o kat :");
    Serial.println(katObrotu);
    for (int a=0; a<iloscObrotow; a++){
      digitalWrite(pulseMinus, HIGH);
      delayMicroseconds(150);
      digitalWrite(pulseMinus, LOW);
      delayMicroseconds(150);
    }
    
    cncStateSignal = digitalRead(cncSignal);
    while (cncStateSignal == HIGH) {
      cncStateSignal = digitalRead(cncSignal);
    }
  }
}

void turnLeftStep(int predkoscObrotowa, int katObrotu, int iloscPowtorzen) {
  for (int a=0; a<iloscPowtorzen; a++){
    cncStateSignal = digitalRead(cncSignal);
    while (cncStateSignal == LOW) {
      cncStateSignal = digitalRead(cncSignal);
    }
    digitalWrite(enableMinus, HIGH);
    delay(20);
    digitalWrite(directionMinus, LOW);
    delay(20);
    
    long iloscObrotow = (long)predkoscObrotowa*katObrotu*2/360;
    Serial.print("Ruch minus o kat :");
    Serial.println(katObrotu);
    for (int a=0; a<iloscObrotow; a++){
      digitalWrite(pulseMinus, HIGH);
      delayMicroseconds(150);
      digitalWrite(pulseMinus, LOW);
      delayMicroseconds(150);
    }
    
    cncStateSignal = digitalRead(cncSignal);
    while (cncStateSignal == HIGH) {
      cncStateSignal = digitalRead(cncSignal);
    }
  }
}

void turnRight() {
  cncStateSignal = digitalRead(cncSignal);
  while (cncStateSignal == LOW) {
    cncStateSignal = digitalRead(cncSignal);
  }
  digitalWrite(enableMinus, HIGH);
  delay(20);
  digitalWrite(directionMinus, HIGH);
  delay(20);
  while(cncStateSignal == HIGH) {
    digitalWrite(pulseMinus, HIGH);
    delayMicroseconds(150);
    digitalWrite(pulseMinus, LOW);
    delayMicroseconds(150);
    cncStateSignal = digitalRead(cncSignal);
  }
}

void turnLeft() {
  cncStateSignal = digitalRead(cncSignal);
  while (cncStateSignal == LOW) {
    cncStateSignal = digitalRead(cncSignal);
  }
  digitalWrite(enableMinus, HIGH);
  delay(20);
  digitalWrite(directionMinus, LOW);
  delay(20);
  while(cncStateSignal == HIGH) {
    digitalWrite(pulseMinus, HIGH);
    delayMicroseconds(150);
    digitalWrite(pulseMinus, LOW);
    delayMicroseconds(150);
    cncStateSignal = digitalRead(cncSignal);
  }
}

void turnZero() {
  cncStateSignal = digitalRead(cncSignal);
  zeroStateSignal = digitalRead(zeroSignal);
  while (cncStateSignal == LOW) {
    cncStateSignal = digitalRead(cncSignal);
  }
  digitalWrite(enableMinus, HIGH);
  delay(20);
  digitalWrite(directionMinus, HIGH);
  delay(20);
  while(zeroStateSignal == LOW) {
    digitalWrite(pulseMinus, HIGH);
    delayMicroseconds(150);
    digitalWrite(pulseMinus, LOW);
    delayMicroseconds(150);
    zeroStateSignal = digitalRead(zeroSignal);
  }
  cncStateSignal = digitalRead(cncSignal);
  while (cncStateSignal == HIGH) {
     cncStateSignal = digitalRead(cncSignal);
  }
}

String readStringValue() {
  if (Serial.available() > 0) {
    String odczytanyString = Serial.readStringUntil('\n');
    delay(20);
    Serial.flush();
    return odczytanyString;
  }
}

int readIntValue() {
  if (Serial.available() > 0) {
    int odczytanyInt = Serial.readStringUntil('\n').toInt();
    delay(20);
    Serial.flush();
    return odczytanyInt;
  }
}

void waitToContinue() {
  while(!Serial.available()) {
  }
}

