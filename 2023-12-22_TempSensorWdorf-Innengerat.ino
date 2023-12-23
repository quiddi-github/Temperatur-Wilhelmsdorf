
//Stand 23.12.2023
//
// Code für ideaspark ESP8266 0.96" OLED Module VR: 2.1
//
// Arduino IDE v1
// Folgenden Link zur Boardverwaltung hinzufügen:
// http://arduino.esp8266.com/stable/package_esp8266com_index.json
// zu finden in der Arduino IDE v1 in Datei/ Voreinstellungen/ Einstellungen/ Zusätzliche Boardverwalter-URLs:
// In der Arduino IDE v1 über Werkzeuge/ Board/ Boardverwalter das Paket "ESP8266 Boards" von ESP8266 Community installieren
// Als Board ESP8266 Boards/ NodeNCU 0.9 (ESP-12 Module) auswählen
//
//
// Hardware
// Temperatursensor DS18B20 ist an Pin D13 angeschlossen.
// Betriebsspannung des Sensors ist 5V und zwischen Data und +5V muss ein 4,7k Widerstand angeschlossen sein.
//
// ! ! ! Zum Schreiben des Programmes bitte den "Boot" Taster beim Übertragen drücken ! ! !

#include <NTP.h>
#include <WiFiUdp.h>
WiFiUDP wifiUdp;
NTP ntp(wifiUdp);

// Anfang: Bibs udn Konfig für W-Lan ESP8266
#include <ESP8266WebServer.h> // <WebServer.h> for ESP32
#define PORT 80
! Hier W-Lan eintragen! const char* ssid = "W-Lan";
! Hier W-Lan Passwort eintragen ! const char* pass = "Passwort";
ESP8266WebServer server(PORT); // WebServer server(PORT); for ESP32
// Ende: Bibs udn Konfig für W-Lan ESP8266

//Anfang: Bibs und Konfig für O-LED 

#define SDA_PIN 12 //GPIO12 / 6
#define SCL_PIN 14 //GPIO14 / 5
#include <Wire.h>                                   // library for I2C comunication
#include "SSD1306Wire.h"                      // library for display
SSD1306Wire display(0x3c,  SDA_PIN, SCL_PIN);   // I2C address of display

//Ende: Bibs und Konfig für O-LED

#include <ThingSpeak.h>  // ThinkSpeak Bib zum lesen der Temperatur
! Hier Channel-ID eintragen ! unsigned long myChannelNumber = 2388427;         //ThingSpeak Channel ID
//const char * myReadAPIKey = "X6PJQU3YJW09LDWF";
WiFiClient  client;

// Variablen für Systemzeiten
unsigned long AktuelleZeit=0;                   //aktuelle Zeit-Variable um delay nicht benutzen zu müssen
unsigned long LetzterLauf=0;
 
void setup(){
// Anfang: O-LED  
display.init();                                         // initializing display
display.flipScreenVertically();               //command to rotate orientation of display
display.clear();   
//Ende: O-LED


  ThingSpeak.begin(client); //ThingSpeak

  //Anfang: W-Lan
  Serial.begin(9600); 
  Serial.print("Verbinden zu: ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
   // Status zur W-Lan-Verbindung auf O-LED anzeigen
  display.setFont(ArialMT_Plain_16);                        // initializing the font type and size
  display.drawString(0, 0, "W-Lan verbinden");               //(  row number , column number, "text")
  display.display();
  String index_w_lan = "";
  int index = 0;
  
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    // Status zur W-Lan-Verbindung auf O-LED anzeigen
    index++;
    index_w_lan = index_w_lan + "ü";
    display.drawString(0, 20,index_w_lan);
    display.display();
    // Variable index resetten, wenn sie größer ist als 12, da der Arduino-Bildschirm nur 12 Zeichen in einer Zeile kann
    if(index > 12) {
      index = 0;
      index_w_lan = "";
      display.clear();
      display.setFont(ArialMT_Plain_16);                        // initializing the font type and size
      display.drawString(0, 0, "W-Lan verbinden");               //(  row number , column number, "text")
      display.display();    
    }
    // Status zur W-Lan Verbindung auf Seriellem-Monitor ausgeben
    Serial.print("ü");
  }
  Serial.println("xx");
  Serial.println("WiFi connected");
  Serial.print("IP-Address of ESP8266 module: ");
  Serial.println(WiFi.localIP());
  // Status zur W-Lan-Verbindung auf O-LED anzeigen
  display.drawString(0, 40,"W-Lan verbunden!");
  display.display();
//Ende: W-Lan
  
  server.begin();
// Für Zeit
    /*
    Zeitzone
    CEST: Central European Summertime
    Beginn europäische Sommerzeit letzter Sonntag im März 2 Uhr GMT + 2 Stunden
  */
  ntp.ruleDST("CEST", Last, Sun, Mar, 2, 120);

  // CET: Central European Time
  // Beginn Normalzeit letzter Sonntag im Oktober 3 Uhr GMT + 1 Stunde
  ntp.ruleSTD("CET", Last, Sun, Oct, 3, 60);

  // ntp starten
  ntp.begin();
}
  
void loop(){
AktuelleZeit = millis();  
if(LetzterLauf > AktuelleZeit){ // Sollte aus unerklärlichen Gründen LetzterLauf größer als AktuelleZeit sein, so setzen wir die Variable zurück
  LetzterLauf = AktuelleZeit;
}
Serial.println(ThingSpeak.readCreatedAt(myChannelNumber));
//String LastCreated = ThingSpeak.readCreatedAt(myChannelNumber);
//int str_len = LastCreated.length() + 1; 
//char char_array[str_len];
//LastCreated.toCharArray(char_array, str_len);
//Serial.println(char_array[12] + char_array[13]);

if ( (AktuelleZeit - LetzterLauf) > 10000){ //alle 10 Sekunden die Daten aktualisieren
float temperatureInF = ThingSpeak.readFloatField(myChannelNumber, 1);
String LastCreated = ThingSpeak.readCreatedAt(myChannelNumber);
ntp.update();
Serial.println("Tag des Monats" + ntp.day());
String stringOne = String(temperatureInF, 2);
String stringTwo = String(ntp.day()) + "." + String(ntp.month()) + "." + String(ntp.year()) + " " + String(ntp.hours()) + ":" + String(ntp.minutes());
Serial.println(temperatureInF);
display.clear();
display.setFont(ArialMT_Plain_16);                        // initializing the font type and size
display.drawString(0, 0, stringTwo);
display.drawString(0, 14, "Temp. Wilhelmsdorf");               //(  row number , column number, "text")
display.drawString(0, 34, " " + stringOne + "°C");
display.setFont(ArialMT_Plain_10); 
display.drawString(0,54, LastCreated);
display.display(); 
LetzterLauf = AktuelleZeit;
}

}
