
# Instructable – Line Follower

Dit stappenplan beschrijft hoe je de line follower opbouwt aan de hand van de Bill of Materials (BOM) en de technische tekeningen.

## Stap 1: Componenten bestellen
Bestel alle componenten die vermeld staan in de Bill of Materials (BOM). Controleer na levering of alle onderdelen aanwezig zijn en overeenkomen met de bestelde specificaties.

## Stap 2: Technische documenten verzamelen
Verzamel de nodige technische tekeningen, elektrische schema's en andere documenten die gebruikt worden tijdens de montage.

## Stap 3: Chassis voorbereiden
1. Neem de gaatjesprint die als basis voor de robot dient.
2. Markeer de posities van de motorbeugels volgens de technische tekening.
3. Bevestig de motorbeugels op de gaatjesprint.
4. Monteer de twee N20-motoren in de motorbeugels.
5. Bevestig de wielen op de motorassen.
6. Monteer het kogellager aan de voor- of achterkant van de robot, afhankelijk van het ontwerp.
7. Controleer of de robot stabiel en vlak staat.

## Stap 4: Elektronica monteren
1. Bevestig de Arduino Nano Every op de gaatjesprint.
2. Monteer de TB6612FNG-motordriver.
3. Bevestig de QTR-8A-sensor aan de voorkant van de robot.
4. Monteer de HC-08 Bluetooth-module.
5. Plaats de start-/stopknop en de aan-/uitschakelaar.
6. Voorzie een bevestiging voor de LiPo-batterij.

## Stap 5: Elektrische verbindingen maken
1. Verbind de QTR-8A-sensor met de Arduino volgens het elektrische schema.
2. Verbind de TB6612FNG-motordriver met de Arduino.
3. Sluit beide motoren aan op de motordriver.
4. Verbind de HC-08 Bluetooth-module met de Arduino.
5. Sluit de start-/stopknop aan op de voorziene ingang.
6. Verbind de batterij met de voedingsschakeling.
7. Zorg ervoor dat alle componenten een gemeenschappelijke GND hebben.
8. Controleer alle verbindingen en polariteiten aan de hand van het elektrische schema.

## Stap 6: Arduino IDE instellen
1. Installeer de Arduino IDE.
2. Installeer de nodige bibliotheken, waaronder de SerialCommands-bibliotheek.
3. Sluit de Arduino Nano Every aan op de computer via USB.
4. Selecteer in de Arduino IDE het juiste board: **Arduino Nano Every**.
5. Selecteer de juiste COM-poort.

## Stap 7: Programma compileren en uploaden
1. Open het Arduino-programma van de line follower.
2. Controleer of de pinconfiguratie overeenkomt met de elektrische aansluitingen.
3. Compileer het programma via de Arduino IDE.
4. Los eventuele compilatiefouten op.
5. Upload het programma naar de Arduino Nano Every.
6. Open de Serial Monitor om te controleren of de Arduino correct opstart.

## Stap 8: Eerste functionele testen
1. Test de QTR-8A-sensor en controleer of alle sensoren afzonderlijk waarden uitlezen.
2. Controleer de communicatie via USB-serieel.
3. Test de draadloze communicatie via de HC-08-module.
4. Controleer of de instellingen correct in het EEPROM-geheugen worden opgeslagen.
5. Test de externe start-/stopknop.
6. Test de TB6612FNG-motordriver en controleer de draairichting van beide motoren.
7. Controleer de werking van de aan-/uitschakelaar.

## Stap 9: Line follower testen
1. Plaats de robot op het voorziene parcours.
2. Controleer of de batterij correct aangesloten en bevestigd is.
3. Schakel de robot in.
4. Start de line follower via de startknop of een seriële opdracht.
5. Controleer of de robot de lijn correct volgt.
6. Pas indien nodig de regelparameters aan.
7. Test de robot op het parcours tot hij drie volledige rondes kan afleggen.

## Stap 10: Eindcontrole
1. Controleer alle soldeerverbindingen.
2. Controleer of alle kabels stevig vastzitten en nergens tegen de wielen komen.
3. Controleer of de batterij stevig bevestigd is.
4. Controleer of alle componenten goed vastzitten.
5. Controleer of de afmetingen van de robot binnen de opgelegde limieten vallen.
6. Voer een laatste test uit op het parcours.
