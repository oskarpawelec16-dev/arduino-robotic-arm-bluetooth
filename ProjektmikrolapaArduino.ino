#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial BTSerial(6, 7);

Servo servoClaw;     // Pin 8  - Łapa / Chwytak
Servo servoRightArm; // Pin 9  - Prawe ramię
Servo servoLeftArm;  // Pin 10 - Lewe ramię
Servo servoBase;     // Pin 11 - Podstawa (obrót)

int posBase = 90;
int posLeftArm = 90;
int posRightArm = 90;
int posClaw = 90;

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);

  servoClaw.attach(8);
  servoRightArm.attach(9);
  servoLeftArm.attach(10);
  servoBase.attach(11);

  resetPositions();
}

void loop() {
  char command = ' ';

  if (BTSerial.available() > 0) {
    command = BTSerial.read();
  } else if (Serial.available() > 0) {
    command = Serial.read();
  }

  if (command != ' ') {
    processCommand(command);
  }
}

void processCommand(char cmd) {
  int armStep = 3;   // Wolny krok dla ramion (brak migania)
  int clawStep = 10; // Mocny krok dla szczypiec

  switch (cmd) {
    // === LEWY PAD (PODSTAWA) ===
    case 'L': case 'l': case '3':
      posBase = constrain(posBase + armStep, 0, 180);
      servoBase.write(posBase);
      break;

    case 'R': case 'r': case '4':
      posBase = constrain(posBase - armStep, 0, 180);
      servoBase.write(posBase);
      break;

    // === LEWE RAMIĘ ===
    case 'U': case 'u': case '1': case 'W': case 'w': case 'F': case 'f':
      posLeftArm = constrain(posLeftArm + armStep, 0, 180);
      servoLeftArm.write(posLeftArm);
      break;

    case 'D': case 'd': case '2': case 'B': case 'b': // Usunięto 'S' i 's' stąd
      posLeftArm = constrain(posLeftArm - armStep, 0, 180);
      servoLeftArm.write(posLeftArm);
      break;

    // === PRAWY PAD (PRAWE RAMIĘ) ===
    case 'T': case 't': case '5': case 'I': case 'i':
      posRightArm = constrain(posRightArm + armStep, 0, 180);
      servoRightArm.write(posRightArm);
      break;

    case 'X': case 'x': case '6': case 'K': case 'k':
      posRightArm = constrain(posRightArm - armStep, 0, 180);
      servoRightArm.write(posRightArm);
      break;

    // === ŁAPA / SZCZYPCE ===
    // Kwadrat / S / Q / J / 7 -> OTWIERANIE
    case 'S': case 's': case 'J': case 'j': case '7': case 'Q': case 'q':
      posClaw = constrain(posClaw + clawStep, 0, 180);
      servoClaw.write(posClaw);
      break;

    // Kółko / C / E / 8 -> ZAMYKANIE
    case 'C': case 'c': case '8': case 'E': case 'e':
      posClaw = constrain(posClaw - clawStep, 0, 180);
      servoClaw.write(posClaw);
      break;

    // === RESET ===
    case 'A': case 'a': case 'P': case 'p':
      resetPositions();
      break;
  }
}

void resetPositions() {
  posBase = 90;
  posLeftArm = 90;
  posRightArm = 90;
  posClaw = 90;
  
  servoBase.write(posBase);
  servoLeftArm.write(posLeftArm);
  servoRightArm.write(posRightArm);
  servoClaw.write(posClaw);
}



