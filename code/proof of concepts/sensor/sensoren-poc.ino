// Sensoren proof of concept: QTR-8A op Arduino Nano Every
// Alle sensoren worden onafhankelijk uitgelezen als ruwe ADC-waarde (0 - 1023).
// Geen calibratie, normalisatie of interpolatie.

const uint8_t NUM_SENSORS = 8;   // minimaal 6 vereist
const uint8_t sensorPins[NUM_SENSORS] = {A0, A1, A2, A3, A4, A5, A6, A7};
int values[NUM_SENSORS];

void setup() {
  Serial.begin(115200);
  // ADC-referentie blijft standaard (VDD = 5 V).
  // De sensor wordt ook op 5 V gevoed, dus het volledige bereik 0 - 1023 is bruikbaar.

  Serial.println("S1\tS2\tS3\tS4\tS5\tS6\tS7\tS8");
}

void loop() {
  for (uint8_t i = 0; i < NUM_SENSORS; i++) {
    values[i] = analogRead(sensorPins[i]);
  }

  for (uint8_t i = 0; i < NUM_SENSORS; i++) {
    Serial.print(values[i]);
    Serial.print('\t');
  }
  Serial.println();

  delay(100);
}