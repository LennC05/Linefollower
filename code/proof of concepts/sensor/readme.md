# Sensoren proof of concept

Dit proof of concept toont aan dat meerdere sensoren van de Pololu QTR-8A onafhankelijk van elkaar uitgelezen kunnen worden, zonder calibratie, normalisatie of interpolatie. De ruwe ADC-waarden worden getoond in de seriële monitor, en het bereik van de AD-converter wordt zo goed mogelijk benut.

## Gebruikte onderdelen

- Arduino Nano Every
- Pololu QTR-8A, 8-kanaals reflectiesensor ([datasheet](https://www.pololu.com/docs/pdf/0j12/qtr-8x.pdf))

## Bedrading

| QTR-8A | Verbonden met |
|---|---|
| VCC | Arduino 5V |
| GND | Arduino GND |
| OUT 1 - OUT 8 | A0 - A7 |
| LEDON | niet aangesloten |

## Werking

- De QTR-8A heeft 8 analoge uitgangen met elk een spanning van 0 V tot Vcc. De uitgangen zijn onafhankelijk van elkaar (datasheet).
- Elke uitgang gaat naar een eigen analoge ingang (A0 - A7). Elke sensor wordt apart uitgelezen met `analogRead()`.
- De waarden worden ruw getoond (0 - 1023), zonder bewerking.
- **Bereik van de AD-converter:** de Nano Every heeft een 10-bit ADC met standaard een referentie van 5 V (VDD). De sensor wordt op 5 V gevoed, dus de uitgang kan tot ongeveer de volle ADC-schaal gaan. Op 3,3 V zou slechts ongeveer twee derde van het bereik benut worden.
- Een witte ondergrond (veel reflectie) geeft een lage waarde, een zwarte ondergrond (weinig reflectie) een hoge waarde.

## Aandachtspunten

- **Afstand:** de optimale afstand tot de ondergrond is 3 mm, maximaal 6 mm. Verder weg wordt het verschil tussen zwart en wit kleiner.
- **Voeding:** de sensor verbruikt ongeveer 100 mA en is afgesteld voor 5 V. Sluit de twee pinnen "3.3V BYPASS" **niet** kort bij 5 V: de LED's kunnen dan doorbranden (datasheet).
- **Omgevingslicht:** infrarood licht uit de omgeving kan de metingen verstoren.
- **LEDON** is intern hoog getrokken, dus de LED's staan standaard aan.

## Testprocedure

1. Sluit de sensor aan volgens de tabel en laad `sensoren-poc.ino` op de Arduino.
2. Open de seriële monitor (115200 baud).
3. Houd de sensor op ongeveer 3 mm boven wit papier en noteer de waarden.
4. Herhaal boven een zwarte lijn (zwarte tape of een zwarte print).
5. Leg een stuk zwarte tape onder één sensor tegelijk en controleer dat enkel die kolom verandert.

- Problemen en oplossingen: **[invullen, of "geen"]**

**Conclusie:** [bv. "Alle sensoren kunnen onafhankelijk uitgelezen worden en het ADC-bereik wordt voor het grootste deel benut."]
