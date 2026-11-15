# Start/stop interrupt proof of concept

Dit proof of concept toont aan dat een start/stop-schakelaar correct werkt met een hardware interrupt. De schakelaar wordt niet continu uitgelezen (polling): de microcontroller wordt onderbroken zodra de schakelaar van stand verandert.

## Beschrijving

Een bool `running` bepaalt of de robot mag rijden. De toestand verandert zodra de schakelaar van stand wisselt: dit wordt opgemerkt door een hardware interrupt op D2, niet door de hoofdlus. De ingebouwde LED toont de toestand (aan = START, uit = STOP).

De pinnen zijn gekozen aan de hand van het elektrisch schema (zie `technische tekeningen`): D2 omdat dit een pin met interruptmogelijkheid is, en de ingebouwde LED op D13 om geen extra onderdelen nodig te hebben.

## Gebruikte onderdelen

- Arduino Nano Every
- SPST mini toggle switch ([datasheet](https://github.com/LennC05/Linefollower/blob/main/datasheets/SPST-toggle-switch.pdf))
- Ingebouwde LED van de Arduino (D13)

## Bedrading

| Schakelaar | Verbonden met |
|---|---|
| Aansluiting 1 | D2 |
| Aansluiting 2 | GND |

Er is geen externe weerstand nodig: de interne pull-up van de Arduino wordt gebruikt (`INPUT_PULLUP`).

## Werking

- Schakelaar gesloten: D2 is LOW, de robot is in toestand **START** en de LED brandt.
- Schakelaar open: D2 is HIGH (pull-up), de robot is in toestand **STOP** en de LED is uit.
- De interrupt wordt gekoppeld aan D2 met `attachInterrupt(..., CHANGE)`. Een toggleschakelaar heeft twee standen, dus de interrupt moet op elke flank (stijgend en dalend) afgaan.
- De interruptroutine (`onSwitchChange`) doet bijna niets: ze noteert alleen dat er iets veranderd is en wanneer. Zo blijft ze zo kort mogelijk.
- **Debounce:** een mechanische schakelaar stuitert, waardoor één schakeling meerdere interrupts kan geven. De `loop()` wacht daarom 30 ms na de laatste flank en leest dan pas de echte stand van de pin uit. Zo is de eindtoestand altijd correct.
- De variabelen die in de interrupt gebruikt worden zijn `volatile`, en `loop()` leest ze met `noInterrupts()` / `interrupts()` zodat een interrupt niet midden in het lezen toeslaat.

## Aandachtspunten

- Gebruik je later een **drukknop** in plaats van een toggleschakelaar, dan gebruik je `FALLING` in plaats van `CHANGE` en wissel je in de code bij elke druk tussen START en STOP.
- Wil je deze schakelaar later de hoofdvoeding laten schakelen, controleer dan dat het de **power-versie** is (7 A bij 30 VDC). De dry-circuit-versie is maar geschikt voor 0,4 VA bij maximaal 20 V (zie datasheet).
- In de uiteindelijke robot zet je bij STOP de motoren uit, bijvoorbeeld door `STBY` van de TB6612FNG laag te maken.

## Testprocedure

1. Laad `start-stop-interrupt-poc.ino` op de Arduino.
2. Open de seriële monitor (115200 baud).
3. Zet de schakelaar op aan en uit en observeer de LED en de seriële monitor.
4. Schakel meerdere keren snel na elkaar en controleer dat de toestand telkens correct is.
