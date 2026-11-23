
#include <EEPROM.h>
#include <SerialCommands.h>

// =================================================
// PIN CONFIGURATIE
// =================================================

// TB6612FNG
const byte PWMA = 5;
const byte AIN1 = 7;
const byte AIN2 = 8;

const byte PWMB = 6;
const byte BIN1 = 9;
const byte BIN2 = 10;

const byte STBY = 4;

// Start/stop knop
const byte START_BUTTON = 2;

// QTR-8A
const byte sensorPins[8] = {
  A0, A1, A2, A3, A4, A5, A6, A7
};

// =================================================
// SERIAL COMMANDS
// =================================================

// USB en Bluetooth hebben elk hun eigen buffer
char usbBuffer[40];
char bluetoothBuffer[40];

SerialCommands usbCommands(
  &Serial, usbBuffer, sizeof(usbBuffer), "\r\n", " "
);

SerialCommands bluetoothCommands(
  &Serial1, bluetoothBuffer,
  sizeof(bluetoothBuffer), "\r\n", " "
);

// Commando's voor USB
void cmdCycleUSB(SerialCommands* sender);
void cmdDebugUSB(SerialCommands* sender);
void cmdStartUSB(SerialCommands* sender);
void cmdStopUSB(SerialCommands* sender);
void cmdSaveUSB(SerialCommands* sender);
void cmdStatusUSB(SerialCommands* sender);

// Commando's voor Bluetooth
void cmdCycleBT(SerialCommands* sender);
void cmdDebugBT(SerialCommands* sender);
void cmdStartBT(SerialCommands* sender);
void cmdStopBT(SerialCommands* sender);
void cmdSaveBT(SerialCommands* sender);
void cmdStatusBT(SerialCommands* sender);

// USB command objects
SerialCommand usbCycle("CYCLE", cmdCycleUSB);
SerialCommand usbDebug("DEBUG", cmdDebugUSB);
SerialCommand usbStart("START", cmdStartUSB);
SerialCommand usbStop("STOP", cmdStopUSB);
SerialCommand usbSave("SAVE", cmdSaveUSB);
SerialCommand usbStatus("STATUS", cmdStatusUSB);

// Bluetooth command objects
SerialCommand btCycle("CYCLE", cmdCycleBT);
SerialCommand btDebug("DEBUG", cmdDebugBT);
SerialCommand btStart("START", cmdStartBT);
SerialCommand btStop("STOP", cmdStopBT);
SerialCommand btSave("SAVE", cmdSaveBT);
SerialCommand btStatus("STATUS", cmdStatusBT);

// =================================================
// INSTELBARE PARAMETERS
// =================================================

unsigned long cycleTime = 10;
bool debugEnabled = false;

// Vaste startwaarde voor de motoren
const int baseSpeed = 100;
const int maxSpeed = 150;

// QTR-8A drempelwaarde
const int lineThreshold = 500;

// PD-regeling
const float KP = 35.0;
const float KD = 12.0;

// Motor richting
const bool LEFT_INVERTED = false;
const bool RIGHT_INVERTED = false;

// =================================================
// EEPROM
// =================================================

struct Settings {
  uint16_t magic;
  uint16_t cycle;
  byte debug;
};

const uint16_t EEPROM_MAGIC = 12345;

Settings settings;

// =================================================
// ALGEMENE VARIABELEN
// =================================================

bool running = false;

volatile bool buttonInterrupt = false;

float previousError = 0;

unsigned long previousCycle = 0;
unsigned long previousDebug = 0;
unsigned long lastButtonTime = 0;

// =================================================
// EEPROM FUNCTIES
// =================================================

void loadSettings() {

  EEPROM.get(0, settings);

  if (settings.magic != EEPROM_MAGIC ||
      settings.cycle < 5 ||
      settings.cycle > 100) {

    cycleTime = 10;
    debugEnabled = false;

    saveSettings();
  }
  else {

    cycleTime = settings.cycle;
    debugEnabled = settings.debug;
  }
}

void saveSettings() {

  settings.magic = EEPROM_MAGIC;
  settings.cycle = cycleTime;
  settings.debug = debugEnabled;

  EEPROM.put(0, settings);
}

// =================================================
// INTERRUPT
// =================================================

void startButtonISR() {

  buttonInterrupt = true;
}

// =================================================
// MOTORSTURING
// =================================================

void setMotor(byte pwmPin, byte in1, byte in2,
              int speed, bool inverted) {

  speed = constrain(speed, -255, 255);

  bool reverse = speed < 0;

  if (inverted) {
    reverse = !reverse;
  }

  digitalWrite(in1, reverse ? LOW : HIGH);
  digitalWrite(in2, reverse ? HIGH : LOW);

  analogWrite(pwmPin, abs(speed));
}

void driveMotors(int leftSpeed, int rightSpeed) {

  leftSpeed = constrain(leftSpeed, -maxSpeed, maxSpeed);
  rightSpeed = constrain(rightSpeed, -maxSpeed, maxSpeed);

  setMotor(PWMA, AIN1, AIN2,
           leftSpeed, LEFT_INVERTED);

  setMotor(PWMB, BIN1, BIN2,
           rightSpeed, RIGHT_INVERTED);
}

void stopMotors() {

  driveMotors(0, 0);
}

// =================================================
// SENSOR UITLEZEN
// =================================================

float readLinePosition(bool &lineFound) {

  long weightedSum = 0;
  long totalValue = 0;

  for (byte i = 0; i < 8; i++) {

    int value = analogRead(sensorPins[i]);

    // Zwarte lijn geeft normaal hogere waarde
    value = max(0, value - lineThreshold);

    weightedSum += (long)value * (i * 1000);
    totalValue += value;
  }

  if (totalValue == 0) {

    lineFound = false;
    return 3500;
  }

  lineFound = true;

  return (float)weightedSum / totalValue;
}

// =================================================
// LINE FOLLOWER
// =================================================

void followLine() {

  bool lineFound;

  float position = readLinePosition(lineFound);

  if (!lineFound) {

    // Zoek de lijn in de laatst bekende richting
    if (previousError >= 0) {
      driveMotors(80, -80);
    }
    else {
      driveMotors(-80, 80);
    }

    return;
  }

  // Middenpositie van de sensorarray = 3500
  float error = (position - 3500) / 1000.0;

  float derivative = error - previousError;

  float correction = KP * error + KD * derivative;

  previousError = error;

  int leftSpeed = baseSpeed + correction;
  int rightSpeed = baseSpeed - correction;

  driveMotors(leftSpeed, rightSpeed);
}

// =================================================
// DEBUG
// =================================================

void debugOutput() {

  if (!debugEnabled) {
    return;
  }

  if (millis() - previousDebug >= 200) {

    previousDebug = millis();

    for (byte i = 0; i < 8; i++) {

      int value = analogRead(sensorPins[i]);

      Serial.print(value);
      Serial.print('\t');
    }

    Serial.println();
  }
}

// =================================================
// COMMAND HANDLERS
// =================================================

// CYCLE <tijd in ms>
void cmdCycle(SerialCommands* sender) {

  char* parameter = sender->Next();

  if (parameter == NULL) {
    sender->GetSerial()->println("Gebruik: CYCLE 10");
    return;
  }

  int newCycle = atoi(parameter);

  if (newCycle >= 5 && newCycle <= 100) {

    cycleTime = newCycle;

    sender->GetSerial()->println("Cycle aangepast");
  }
  else {

    sender->GetSerial()->println("Cycle moet 5-100 ms zijn");
  }
}

// DEBUG ON / DEBUG OFF
void cmdDebug(SerialCommands* sender) {

  char* parameter = sender->Next();

  if (parameter == NULL) {
    sender->GetSerial()->println("Gebruik: DEBUG ON/OFF");
    return;
  }

  if (strcmp(parameter, "ON") == 0) {

    debugEnabled = true;
    sender->GetSerial()->println("Debug ON");
  }
  else if (strcmp(parameter, "OFF") == 0) {

    debugEnabled = false;
    sender->GetSerial()->println("Debug OFF");
  }
  else {

    sender->GetSerial()->println("Gebruik: DEBUG ON/OFF");
  }
}

// START
void cmdStart(SerialCommands* sender) {

  running = true;
  previousError = 0;

  sender->GetSerial()->println("START");
}

// STOP
void cmdStop(SerialCommands* sender) {

  running = false;
  stopMotors();

  sender->GetSerial()->println("STOP");
}

// SAVE
void cmdSave(SerialCommands* sender) {

  saveSettings();

  sender->GetSerial()->println("EEPROM opgeslagen");
}

// STATUS
void cmdStatus(SerialCommands* sender) {

  sender->GetSerial()->print("Cycle: ");
  sender->GetSerial()->println(cycleTime);

  sender->GetSerial()->print("Debug: ");
  sender->GetSerial()->println(debugEnabled ? "ON" : "OFF");

  sender->GetSerial()->print("Running: ");
  sender->GetSerial()->println(running ? "YES" : "NO");
}

// USB wrappers
void cmdCycleUSB(SerialCommands* s) { cmdCycle(s); }
void cmdDebugUSB(SerialCommands* s) { cmdDebug(s); }
void cmdStartUSB(SerialCommands* s) { cmdStart(s); }
void cmdStopUSB(SerialCommands* s) { cmdStop(s); }
void cmdSaveUSB(SerialCommands* s) { cmdSave(s); }
void cmdStatusUSB(SerialCommands* s) { cmdStatus(s); }

// Bluetooth wrappers
void cmdCycleBT(SerialCommands* s) { cmdCycle(s); }
void cmdDebugBT(SerialCommands* s) { cmdDebug(s); }
void cmdStartBT(SerialCommands* s) { cmdStart(s); }
void cmdStopBT(SerialCommands* s) { cmdStop(s); }
void cmdSaveBT(SerialCommands* s) { cmdSave(s); }
void cmdStatusBT(SerialCommands* s) { cmdStatus(s); }

// =================================================
// SETUP
// =================================================

void setup() {

  // USB communicatie
  Serial.begin(115200);

  // HC-08 Bluetooth communicatie
  Serial1.begin(9600);

  // Motor driver
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  pinMode(STBY, OUTPUT);

  digitalWrite(STBY, HIGH);

  // Sensoren
  for (byte i = 0; i < 8; i++) {
    pinMode(sensorPins[i], INPUT);
  }

  // Startknop
  pinMode(START_BUTTON, INPUT_PULLUP);

  attachInterrupt(
    digitalPinToInterrupt(START_BUTTON),
    startButtonISR,
    FALLING
  );

  // EEPROM
  loadSettings();

  // USB commands registreren
  usbCommands.AddCommand(&usbCycle);
  usbCommands.AddCommand(&usbDebug);
  usbCommands.AddCommand(&usbStart);
  usbCommands.AddCommand(&usbStop);
  usbCommands.AddCommand(&usbSave);
  usbCommands.AddCommand(&usbStatus);

  // Bluetooth commands registreren
  bluetoothCommands.AddCommand(&btCycle);
  bluetoothCommands.AddCommand(&btDebug);
  bluetoothCommands.AddCommand(&btStart);
  bluetoothCommands.AddCommand(&btStop);
  bluetoothCommands.AddCommand(&btSave);
  bluetoothCommands.AddCommand(&btStatus);

  stopMotors();

  Serial.println("Line follower ready");
  Serial1.println("Line follower ready");
}

// =================================================
// LOOP
// =================================================

void loop() {

  // Communicatie verwerken
  usbCommands.ReadSerial();
  bluetoothCommands.ReadSerial();

  // Externe interrupt verwerken
  if (buttonInterrupt) {

    noInterrupts();
    buttonInterrupt = false;
    interrupts();

    if (millis() - lastButtonTime > 250) {

      lastButtonTime = millis();

      running = !running;

      if (!running) {
        stopMotors();
      }
      else {
        previousError = 0;
      }
    }
  }

  // Cyclische uitvoering
  if (running && millis() - previousCycle >= cycleTime) {

    previousCycle = millis();

    followLine();
    debugOutput();
  }

  // Motoren altijd stoppen wanneer niet actief
  if (!running) {
    stopMotors();
  }
}

