// Start/stop met hardware interrupt
// SPST toggle switch op D2 (naar GND), ingebouwde LED op D13 toont de toestand

const uint8_t SWITCH_PIN = 2;
const uint8_t LED_PIN = LED_BUILTIN;
const unsigned long DEBOUNCE_MS = 30;

volatile bool changeFlag = false;
volatile unsigned long lastEdgeTime = 0;
volatile unsigned long interruptCount = 0;

bool running = false;
unsigned long lastHeartbeat = 0;

// Interrupt service routine: zo kort mogelijk, alleen noteren dat er iets veranderd is
void onSwitchChange() {
  lastEdgeTime = millis();
  changeFlag = true;
  interruptCount++;
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);   // schakelaar gesloten = LOW = START

  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), onSwitchChange, CHANGE);

  running = (digitalRead(SWITCH_PIN) == LOW);
  digitalWrite(LED_PIN, running ? HIGH : LOW);
  Serial.println(running ? "Beginstand: START" : "Beginstand: STOP");
}

void loop() {
  bool process = false;
  unsigned long count;

  // Gedeelde variabelen veilig lezen (niet onderbroken door de interrupt)
  noInterrupts();
  if (changeFlag && (millis() - lastEdgeTime >= DEBOUNCE_MS)) {
    changeFlag = false;
    process = true;
  }
  count = interruptCount;
  interrupts();

  // Pas verwerken als de schakelaar uitgestuiterd is
  if (process) {
    bool newState = (digitalRead(SWITCH_PIN) == LOW);
    if (newState != running) {
      running = newState;
      digitalWrite(LED_PIN, running ? HIGH : LOW);
      Serial.print(running ? "START" : "STOP");
      Serial.print("  (aantal interrupts tot nu toe: ");
      Serial.print(count);
      Serial.println(")");
    }
  }

  // Hoofdprogramma: laat zien dat de loop gewoon doorloopt
  if (running && millis() - lastHeartbeat >= 1000) {
    lastHeartbeat = millis();
    Serial.println("robot actief...");
  }
}