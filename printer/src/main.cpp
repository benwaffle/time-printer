#include <Adafruit_Thermal.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define TX_PIN D6 // Arduino transmit  YELLOW WIRE  labeled RX on printer
#define RX_PIN D5 // Arduino receive   GREEN WIRE   labeled TX on printer

SoftwareSerial mySerial(RX_PIN, TX_PIN);
Adafruit_Thermal printer(&mySerial);
WiFiClient client;
HTTPClient http;

void setup() {
  mySerial.begin(19200);
  printer.begin();

  Serial.begin(115200);

  Serial.println(WiFi.macAddress());

  WiFi.begin("Stevens-Media", "Stevens1870");

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
  if (res == 200) {
    printer.wake();
    printer.setDefault();
    printer.println(payload);
    printer.feed(2);
    printer.sleep();
  }
  delay(5000L);
}
