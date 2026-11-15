# H-brug proof of concept

Dit proof of concept toont aan dat twee motoren onafhankelijk van elkaar kunnen draaien en dat hun snelheid (traploos) en draairichting regelbaar zijn. Er wordt een TB6612FNG als H-brug gebruikt, aangestuurd door een Arduino Nano Every.

## Gebruikte onderdelen

- Arduino Nano Every
- TB6612FNG dual H-brug ([datasheet](LennC05/Linefollower/datasheets/TB6612FNG.pdf))
- 2x N20 DC gearmotor 6 V 500 RPM
- 2S LiPo 7,4 V 850 mAh met schakelaar

## Bedrading

| TB6612FNG | Verbonden met |
|---|---|
| VM | LiPo plus (via schakelaar) |
| GND | LiPo min en Arduino GND |
| VCC | Arduino 5V |
| STBY | D4 |
| AIN1 / AIN2 / PWMA | D7 / D8 / D5 |
| BIN1 / BIN2 / PWMB | D11 / D12 / D6 |
| AO1 / AO2 | Motor A |
| BO1 / BO2 | Motor B |

## Aandachtspunten

- **STBY moet hoog zijn.** Staat STBY laag, dan staat de driver in standby en schakelen de uitgangen niet (datasheet).
- **Gemeenschappelijke GND.** LiPo, driver en Arduino delen dezelfde massa, anders werkt de aansturing niet.
- **Spanning.** VM mag 4,5 - 13,5 V zijn. De motoren zijn 6 V, daarom is de PWM begrensd (`MAX_PWM`).
- **PWM-pinnen.** D5 en D6 zijn PWM-pinnen van de Nano Every. (Nog uit te zoeken: gebruiken ze dezelfde timer en dus dezelfde frequentie?)
- **Geen foutuitgang.** De TB6612FNG heeft ingebouwde thermische beveiliging maar geeft geen foutsignaal aan de Arduino.

## Werking van de code

- `setMotor(motor, percent)` stuurt één motor aan. Een waarde tussen -100 en +100 bepaalt snelheid en richting: positief is vooruit, negatief achteruit, 0 is stop.
- De richting wordt bepaald door de pinnen IN1 en IN2, de snelheid door een PWM-signaal.
- De motoren zijn 6 V maar de LiPo levert 7,4 - 8,4 V. Daarom is de PWM begrensd op 180 van 255 (`MAX_PWM`).
- `ramp(motor, van, naar)` verandert de snelheid stapsgewijs, zodat de traploze regeling zichtbaar is.

## Testprocedure

1. Laad `h-bridge-poc.ino` op de Arduino, zonder LiPo aangesloten.
2. Open de seriële monitor (115200 baud) en controleer dat de tekstregels verschijnen.
3. Controleer de bedrading, vooral de gemeenschappelijke GND.
4. Zet de LiPo aan.
5. Observeer de drie fasen: motor A alleen, motor B alleen, beide tegelijk met verschillende snelheid en richting.

## Resultaat

(Vul in na de test: werkte alles? Vanaf welk percentage start de motor? Welke problemen kwamen voor?)

![Opstelling](../../images/h-brug-poc.jpg)

(Link naar een filmpje van de test, indien beschikbaar.)
