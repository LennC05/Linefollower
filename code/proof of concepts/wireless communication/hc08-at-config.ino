// HC-08 AT-configuratie: geeft alles door tussen USB en de HC-08
// Seriële monitor: 115200 baud. Geen telefoon verbinden tijdens het configureren.

void setup() {
  Serial.begin(115200);    // USB naar laptop
  Serial1.begin(9600);     // HC-08 (standaard 9600 baud)
  Serial.println("AT-modus: typ AT en verstuur.");
}

void loop() {
  if (Serial1.available()) Serial.write(Serial1.read());
  if (Serial.available()) Serial1.write(Serial.read());
}