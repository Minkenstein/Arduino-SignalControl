//-------------------
//|Globale Variablen|
//-------------------
int randomSignal = 0;
int oldRandomSignal = 0;
String serialString = "0";
int randomLED = 2;
int oldRandomLED = 0;

// Zeiten für Signal wechsel.
const int HP0_TIME = 8000;
const int SIGNAL_TIME = 15000;

// Auswahl des Modus der starten soll.
const int SIGNAL_MODE = 0;
// Mode 0 - Random Signalbild
// Mode 1 - Steuerung per Serial
// Mode 2 - Party-Modus

//----------------
//|Setup und Loop|
//----------------
void setup() { 
  defineIO();
  setPinmode();
  checkLED();
  checkFade();
// Vorbereitungen für Modus
  switch (SIGNAL_MODE){
    case 1:
      Serial.begin(9600);
      break;
  }
changeSignalbild(0);
}

void loop() {
  // Loop je nach Modus.
  switch (SIGNAL_MODE){
    case 0:
      getRandomSignal();
      break;
    case 1:
      serialControl();
      break;
    case 2:
      partyModus();
      break;
  }
}

//------------
//|Funktionen|
//------------
void defineIO(){
  // Funktion zum definieren von Pins.
  // Namen für die LEDs am Hauptsignal.
  #define HP_RED_1 2
  #define HP_RED_2 3
  #define HP_GREEN 4
  #define HP_ORANGE 5
  #define HP_WHITE 6
  // Namen für die LEDs am Vorsignal.
  #define VR_GREEN_1 7
  #define VR_GREEN_2 8
  #define VR_ORANGE_1 9
  #define VR_ORANGE_2 10
  // LEDs am Signal haben gemeinsamen Ground. Deswegen ist HIGH gleich LED aus.
  #define AN 0
  #define AUS 1
  // Pin für die Steurung des Transistors.
  #define FADEPIN 11
}
void setPinmode(){
  // Hier setzten wir alle Pinmodes.
  for(int pin = 2; pin <= 11; pin++){
    pinMode(pin, OUTPUT);
    digitalWrite(pin, AUS);
  }
  analogWrite(11, 0);
}
void checkLED(){
  // Funktion zum Checken der LEDs.
  for(int pin = 2; pin <= 10; pin++){    
    digitalWrite(pin, AN);
    delay(250);
    digitalWrite(pin, AUS);
  }
}
void checkFade(){
  // Funktion zum Checken vom Fade der LEDs.
  for(int pin = 2; pin <= 11; pin++){
    pinMode(pin, OUTPUT);
    digitalWrite(pin, AN);
  }
  for (int bright = 255; bright >= 0; bright--){
    analogWrite(FADEPIN, bright);
    delay(5);
    }
  for (int bright = 0; bright <= 255; bright++){
    analogWrite(FADEPIN, bright);
    delay(5);
    }
  for(int pin = 2; pin <= 11; pin++){
    pinMode(pin, OUTPUT);
    digitalWrite(pin, AUS);
  }
}
void changeSignalbild(int sBild){
  // Funktion zum ändern des Sginalbildes.
  // LEDs ausfaden und LEDs ausschalten.
  for (int bright = 0; bright <= 255; bright++){
    analogWrite(FADEPIN, bright);
    delayMicroseconds(700);
    }
  for(int pin = 2; pin <= 10; pin++){    
    digitalWrite(pin, AUS);
    }
  // Ändern des Signalbildes.
  switch(sBild){
    case 0: //HP0
      digitalWrite(HP_RED_1, AN);
      digitalWrite(HP_RED_2, AN);
      break;
    case 1: //HP1VR0
      digitalWrite(HP_GREEN, AN);
      digitalWrite(VR_ORANGE_1, AN);
      digitalWrite(VR_ORANGE_2, AN);
      break;
    case 2: //HP1VR1
      digitalWrite(HP_GREEN, AN);
      digitalWrite(VR_GREEN_1, AN);
      digitalWrite(VR_GREEN_2, AN);
      break;
    case 3: //HP1VR2
      digitalWrite(HP_GREEN, AN);
      digitalWrite(VR_GREEN_1, AN);
      digitalWrite(VR_ORANGE_2, AN);
      break;
    case 4: //HP2VR0
      digitalWrite(HP_GREEN, AN);
      digitalWrite(HP_ORANGE, AN);
      digitalWrite(VR_ORANGE_1, AN);
      digitalWrite(VR_ORANGE_2, AN);
      break;
    case 5: //HP2VR1
      digitalWrite(HP_GREEN, AN);
      digitalWrite(HP_ORANGE, AN);
      digitalWrite(VR_GREEN_1, AN);
      digitalWrite(VR_GREEN_2, AN);
      break;
    case 6: //HP2VR2
      digitalWrite(HP_GREEN, AN);
      digitalWrite(HP_ORANGE, AN);
      digitalWrite(VR_GREEN_1, AN);
      digitalWrite(VR_ORANGE_2, AN);
      break;
    case 7: //SH1
      digitalWrite(HP_RED_1, AN);
      digitalWrite(HP_WHITE, AN);
      break;
    case 8: // LED-Test Hauptsignal
      digitalWrite(HP_RED_1, AN);
      digitalWrite(HP_RED_2, AN);
      digitalWrite(HP_GREEN, AN);
      digitalWrite(HP_ORANGE, AN);
      digitalWrite(HP_WHITE, AN);
      break;
    case 9: // LED-Test Vorsignal
      digitalWrite(VR_GREEN_1, AN);
      digitalWrite(VR_GREEN_2, AN);
      digitalWrite(VR_ORANGE_1, AN);
      digitalWrite(VR_ORANGE_2, AN);
      break;
    case 10: // LED-Test Haupt- und Vorsignal
      digitalWrite(HP_RED_1, AN);
      digitalWrite(HP_RED_2, AN);
      digitalWrite(HP_GREEN, AN);
      digitalWrite(HP_ORANGE, AN);
      digitalWrite(HP_WHITE, AN);
      digitalWrite(VR_GREEN_1, AN);
      digitalWrite(VR_GREEN_2, AN);
      digitalWrite(VR_ORANGE_1, AN);
      digitalWrite(VR_ORANGE_2, AN);
      break;
    default: //HP0
      digitalWrite(HP_RED_1, AN);
      digitalWrite(HP_RED_2, AN);
      break;   
    }
  // LEDs langsam anschalten.
  for (int bright = 255; bright >= 0; bright--){
      analogWrite(FADEPIN, bright);
      delayMicroseconds(700);
    } 
}
void getRandomSignal(){
  // Zuffäliges Signalbild, beim wechseln wieder in HP0 zurück.
  randomSignal = random(1,8);
  if(randomSignal == oldRandomSignal){
    return;
  }
  oldRandomSignal = randomSignal;
  changeSignalbild(randomSignal);    
  delay(SIGNAL_TIME);
  changeSignalbild(0);
  delay(HP0_TIME);
}
void serialControl(){
  // Holt sich die Werte von Serial und setzt es in ein Signalbild um.
  if (Serial.available() > 0) {
    serialString = Serial.readString();
    changeSignalbild(serialString.toInt());
    Serial.println("Wechsel zu Signalbild Nr.: " + serialString);
    }
}
void partyModus(){
  // Party-Modus mit tollen bunten LEDs.
  randomLED = random(2,11);
  if (oldRandomLED == randomLED){
    return;
  } else {
    digitalWrite(randomLED, AN);
    delay(50);
    digitalWrite(randomLED, AUS);
  } 
}
