/*
    HTTP over TLS (HTTPS) example sketch

    This example demonstrates how to use
    WiFiClientSecure class to access HTTPS API.
    We fetch and display the status of
    esp8266/Arduino project continuous integration
    build.

    Limitations:
      only RSA certificates
      no support of Perfect Forward Secrecy (PFS)
      TLSv1.2 is supported since version 2.4.0-rc1

    Created by Ivan Grokhotkov, 2015.
    This example is in public domain.

    Move a dot edit by Jonas Vorwerk 2019
    
*/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>        //V5! https://github.com/bblanchon/ArduinoJson
#include <FastLED.h>            //https://github.com/FastLED/FastLED 
FASTLED_USING_NAMESPACE

#define DEBUG 1


#ifndef STASSID
//#define STASSID "Network"
//#define STAPSK  "chantalislief"
#define STASSID "DE KROON"
#define STAPSK  "Kroondomein20-22"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

String url = "/test/json2.php";
const char* host = "moveadot.nl";
const int httpsPort = 443;

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
const char fingerprint[] PROGMEM = "4B 06 33 A8 56 C3 E4 DA B3 4F 76 6B 5D BF 4D DB F0 12 47 EE";

#define WEB_UPDATE_TIME 10





//------------JSON
//ArduinoJson 5
StaticJsonBuffer<400> JSONbuffer;
JsonObject& JSONencoder = JSONbuffer.createObject();
char JSONmessageBuffer[400];





//----------FASTLED
#define NUM_LEDS 32
CRGB leds[NUM_LEDS];

boolean power = 1;
uint8_t hue =  20;
uint8_t sat = 255;
uint8_t val = 200;
uint8_t fadeout = 128; 






void setup() {
  Serial.begin(115200);
  
  FastLED.addLeds<NEOPIXEL, 15 >(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness(32);
  FastLED.clear();
  
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  init_all_dots();
  
}

void loop() {
  EVERY_N_SECONDS( WEB_UPDATE_TIME ) {
    getDataFromWebsite();
  }

  move_a_dot();

  FastLED.show();
  FastLED.delay(1000 / 60);
}
