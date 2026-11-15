// H-brug proof of concept: 2 motoren met TB6612FNG op Arduino Nano Every

const uint8_t STBY = 4;
const uint8_t AIN1 = 7, AIN2 = 8, PWMA = 5;
const uint8_t BIN1 = 11, BIN2 = 12, PWMB = 6;

// Motoren zijn 6 V, LiPo is 7,4 - 8,4 V: PWM begrenzen
const int MAX_PWM = 180;

// motor: 'A' of 'B', percent: -100 (vol achteruit) tot +100 (vol vooruit)
void setMotor(char motor, int percent) {
  percent = constrain(percent, -100, 100);

  uint8_t in1 = (motor == 'A') ? AIN1 : BIN1;
  uint8_t in2 = (motor == 'A') ? AIN2 : BIN2;
  uint8_t pwm = (motor == 'A') ? PWMA : PWMB;

  if (percent > 0) {            // vooruit
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  } else if (percent < 0) {     // achteruit
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  } else {                      // stop
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }
  analogWrite(pwm, map(abs(percent), 0, 100, 0, MAX_PWM));
}

// verandert de snelheid van een motor geleidelijk (traploos)
void ramp(char motor, int from, int to) {
  int step = (to > from) ? 1 : -1;
  for (int p = from; p != to; p += step) {
    setMotor(motor, p);
    delay(20);
  }
  setMotor(motor, to);
}

void setup() {
  Serial.begin(115200);
  pinMode(STBY, OUTPUT);
  pinMode(AIN1, OUTPUT); pinMode(AIN2, OUTPUT); pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT); pinMode(BIN2, OUTPUT); pinMode(PWMB, OUTPUT);
  digitalWrite(STBY, HIGH);   // driver actief
  setMotor('A', 0);
  setMotor('B', 0);
  delay(2000);                // tijd om de LiPo aan te zetten
}

void loop() {
  Serial.println("Motor A: 0 -> +100 -> -100 -> 0");
  ramp('A', 0, 100);
  ramp('A', 100, -100);
  ramp('A', -100, 0);
  delay(1000);

  Serial.println("Motor B: 0 -> +100 -> -100 -> 0");
  ramp('B', 0, 100);
  ramp('B', 100, -100);
  ramp('B', -100, 0);
  delay(1000);

  Serial.println("A en B tegelijk, verschillende snelheid en richting");
  setMotor('A', 60);
  setMotor('B', -30);
  delay(3000);
  setMotor('A', -30);
  setMotor('B', 60);
  delay(3000);
  setMotor('A', 0);
  setMotor('B', 0);
  delay(3000);
}