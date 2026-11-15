# Draadloze communicatie proof of concept

Dit proof of concept toont aan dat duplex gecommuniceerd kan worden tussen de microcontroller en een **smartphone**, met een HC-08 (Bluetooth 4.0 BLE) en de app **nRF Connect for Mobile**. De smartphone stuurt commando's naar de Arduino en de Arduino stuurt antwoorden en statusberichten terug.

## Gebruikte onderdelen

- Arduino Nano Every
- HC-08 Bluetooth 4.0 BLE ([datasheet](https://wds-service-1258344699.file.myqcloud.com/20/12636/pdf/16705727870380831e4085d285073.pdf))
- 3 weerstanden van 1 kΩ (spanningsdeler)
- Smartphone met de app nRF Connect for Mobile

## Bedrading

| HC-08 | Arduino Nano Every |
|---|---|
| VCC | 3V3 |
| GND | GND |
| TX | D0 (RX) |
| RX | D1 (TX), via een spanningsdeler |

Spanningsdeler: D1 → 1 kΩ → HC-08 RX, en van HC-08 RX → 2 kΩ (twee weerstanden van 1 kΩ in serie) → GND. Zo wordt de 5 V van de Arduino omgezet naar ongeveer 3,3 V, want de seriële niveaus van de HC-08 zijn 3,3 V.

## Werking

- De HC-08 is een UART-module met BLE. De Arduino gebruikt de hardware-seriële poort `Serial1` (D0/D1) om te praten met de HC-08. De USB-poort (`Serial`) wordt apart gebruikt voor debuggen.
- **Smartphone naar Arduino:** de commando's `1` (LED aan), `0` (LED uit) en `?` (status opvragen).
- **Arduino naar smartphone:** een antwoord op elk commando, en elke 2 seconden een statusbericht met teller, LED-toestand en uptime.
- Beide richtingen werken tegelijk, dus de communicatie is duplex.

## Configuratie

De module wordt ingesteld met AT-commando's. Dit werkt enkel als er **geen** telefoon verbonden is.

1. Laad `code/hc08-at-config/hc08-at-config.ino` op de Arduino.
2. Open de seriële monitor op 115200 baud.
3. Stuur de volgende commando's, met ongeveer 1 seconde tussen elk commando:

| Commando | Antwoord | Doel |
|---|---|---|
| `AT` | `OK` | Verbinding testen |
| `AT+NAME?` | `OK+NAME:...` | Huidige naam opvragen (standaard `SH-HC-08`) |
| `AT+NAME[naam]` | `OK+Set:[naam]` | Eigen naam instellen, zodat je hem herkent in de app |
| `AT+VERSION` | versie-info | Versie opvragen |

De standaardinstellingen zijn 9600 baud en wachtwoord 000000. De baudrate blijft ongewijzigd.

4. Laad daarna `draadloze-communicatie-poc.ino` op de Arduino.

## Gebruiksaanwijzing

1. Sluit de HC-08 aan zoals in de tabel en laad `draadloze-communicatie-poc.ino` op de Arduino.
2. Open op de smartphone **nRF Connect for Mobile** en tik op **Scan**.
3. Zoek de module in de lijst (naam `SH-HC-08` of je eigen naam) en tik op **Connect**. Dit doe je in de app, niet in de Bluetooth-instellingen van de telefoon.
4. Open de dienst met UUID **FFE0** en de karakteristiek **FFE1**.
5. Tik op het icoon met de drie pijltjes om meldingen (notifications) in te schakelen. Zo ontvang je de berichten van de Arduino.
6. Tik op het icoon met de pijl omhoog (write), kies het type tekst (UTF-8), typ `1`, `0` of `?` en verstuur.
7. De LED op de Arduino brandt of dooft, en je ziet het antwoord in de app.

## Opmerkingen

- De HC-08 is een **BLE**-module. Hij verschijnt niet in de klassieke Bluetooth-instellingen van de telefoon, enkel in een BLE-app.
- De voeding is 3,3 V. De seriële niveaus zijn eveneens 3,3 V (datasheet), daarom de spanningsdeler op de RX van de module.
- AT-commando's werken alleen zolang er geen verbinding is (datasheet).
- Het bereik is volgens de datasheet ongeveer 20 m in open ruimte. Metaal, muren en bomen verzwakken het signaal.
- Tijdens het gebruik van de module wordt `Serial1` gebruikt, dus D0 en D1 zijn niet beschikbaar voor andere toepassingen.

- Problemen en oplossingen: **[invullen, of "geen"]**
