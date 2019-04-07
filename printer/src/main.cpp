#include <Adafruit_Thermal.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#define TX_PIN 6 // Arduino transmit  YELLOW WIRE  labeled RX on printer
#define RX_PIN 5 // Arduino receive   GREEN WIRE   labeled TX on printer

SoftwareSerial mySerial(RX_PIN, TX_PIN); // Declare SoftwareSerial obj first
Adafruit_Thermal printer(&mySerial);     // Pass addr to printer constructor
WiFiClient client;
HTTPClient http;

void setup() {
  mySerial.begin(19200);  // Initialize SoftwareSerial
  printer.begin();        // Init printer (same regardless of serial type)

  Serial.begin(9600);

  WiFi.begin("toasters don't toast toast", "toast toast toast");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("ip: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  http.begin(client, "http://iofel.me:1337");
  int res = http.GET();
  String payload = http.getString();
  http.end();
  Serial.println(res);
  Serial.println(payload);
  // printer.println("@benwafflez: tweet");
  // printer.feed(2);
  // printer.sleep();      // Tell printer to sleep
  delay(3000L);         // Sleep for 3 seconds
  // printer.wake();       // MUST wake() before printing again, even if reset
  // printer.setDefault(); // Restore printer to defaults
}
