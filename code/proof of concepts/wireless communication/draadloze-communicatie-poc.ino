// Draadloze communicatie proof of concept: HC-08 (BLE) op Serial1 van de Nano Every
// Smartphone -> Arduino: commando's  1 = LED aan, 0 = LED uit, ? = status opvragen
// Arduino -> smartphone: antwoord op elk commando + elke 2 s een statusbericht

const unsigned long STATUS_INTERVAL = 2000;
unsigned long lastStatus = 0;
unsigned long messageCount = 0;

void sendStatus() {
  messageCount++;
  Serial1.print("Status #");
  Serial1.print(messageCount);
  Serial1.print(" | LED: ");
  Serial1.print(digitalRead(LED_BUILTIN) ? "aan" : "uit");
  Serial1.print(" | uptime: ");
  Serial1.print(millis() / 1000);
  Serial1.println(" s");
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);    // USB naar laptop (debug)
  Serial1.begin(9600);     // HC-08
  Serial.println("Start");
  Serial1.println("Arduino klaar");
}

void loop() {
  // 1. Ontvangen: smartphone -> Arduino
  while (Serial1.available() > 0) {
    char c = Serial1.read();
    switch (c) {
      case '1':
        digitalWrite(LED_BUILTIN, HIGH);
        Serial1.println("LED aan");
        break;
      case '0':
        digitalWrite(LED_BUILTIN, LOW);
        Serial1.println("LED uit");
        break;
      case '?':
        sendStatus();
        break;
      case '\r':
      case '\n':
        break;           // regeleinden negeren
      default:
        Serial1.print("Onbekend commando: ");
        Serial1.println(c);
    }
    Serial.print("Ontvangen: ");
    Serial.println(c);
  }

  // 2. Zenden: Arduino -> smartphone (elke 2 s)
  if (millis() - lastStatus >= STATUS_INTERVAL) {
    lastStatus = millis();
    sendStatus();
  }
}