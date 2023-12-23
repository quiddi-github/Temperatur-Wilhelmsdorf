"# Temperatur-Wilhelmsdorf" 

Stand: 23.12.2023\
\
Es gibt zwei Geräte,\
ein Außengerät\
 - AZ-Delivery mit ESP32-WROOM-32\
 - inkl. TemperaturSensor DS18B20\
 
ein Innengerät mit OLED-Display\
 - ideaspark ESP8266 0.96" OLED Module VR:2.1\

Das Außengerät liest über den DS18B20 die Temperatur und schickt sie über W-Lan an den Thingspeak-Server. Das Innengerät liest den Wert auf dem ThingSpeak-Server und zeigt ihn auf dem OLED-Monitor an.\
\
Die Boardinformationen sind den Arduino-Sketches zu entnehmen. Es wurde die Arduino IDE v1.8.19 verwendet.