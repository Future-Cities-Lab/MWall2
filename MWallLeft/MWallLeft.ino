#include "FastLED.h"
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>   

EthernetUDP Udp;
IPAddress ip;

unsigned int localPort = 2222;  
byte mac[] = { 0x04, 0xE9, 0xE5, 0x03, 0x18, 0xFF };

const unsigned int resetPin = 9;

const unsigned int ledPin0 = 2;
const unsigned int ledPin1 = 5;
const unsigned int ledPin2 = 6;
const unsigned int ledPin3 = 21;
const unsigned int ledPin4 = 7;

const unsigned int n_Leds_1a = 127;
const unsigned int n_Leds_1b = 128;
const unsigned int n_Leds_1c = 130;
const unsigned int ttl_Sct1 = 385;

const unsigned int n_Leds_2a = 175;
const unsigned int n_Leds_2b = 174;
const unsigned int n_Leds_2c = 176;
const unsigned int ttl_Sct2 = 525;

const unsigned int ttl_Sct3 = 629;
const unsigned int ttl_Sct4 = 559;
const unsigned int ttl_Sct5 = 743;

const unsigned int ttl = ttl_Sct1 + ttl_Sct2 + ttl_Sct3 + ttl_Sct4 + ttl_Sct5;

CRGB leds[ttl];

const int buff_sz_1a = n_Leds_1a*3;
const int buff_sz_1b = n_Leds_1b*3;
const int buff_sz_1c = n_Leds_1c*3;

char pck_Bff_1[buff_sz_1a];
char pck_Bff_1b[buff_sz_1b];
char pck_Bff_1c[buff_sz_1c];

const int buff_sz_2a = n_Leds_2a*3;
const int buff_sz_2b = n_Leds_2b*3;
const int buff_sz_2c = n_Leds_2c*3;

char pck_Bff_2a[buff_sz_2a];
char pck_Bff_2b[buff_sz_2b];
char pck_Bff_2c[buff_sz_2c];

void setup() {
  delay(2000);
  digitalWrite(resetPin, LOW);
  pinMode(resetPin, OUTPUT);
  delayMicroseconds(10);
  pinMode(resetPin, INPUT);
  ip = IPAddress(192,168,1,110);
  Ethernet.begin(mac, ip);
  delay(200);
  Serial.begin(9600);
  Udp.begin(localPort);
  
  FastLED.addLeds<NEOPIXEL, ledPin0>(leds, 0, ttl_Sct1);
  FastLED.addLeds<NEOPIXEL, ledPin1>(leds, ttl_Sct1, ttl_Sct2);
//  FastLED.addLeds<NEOPIXEL, ledPin2>(leds, ttl_Sct1+ttl_Sct2, ttl_Sct3);
//  FastLED.addLeds<NEOPIXEL, ledPin3>(leds, ttl_Sct1+ttl_Sct2+ttl_Sct3, ttl_Sct4);
//  FastLED.addLeds<NEOPIXEL, ledPin4>(leds, ttl_Sct1+ttl_Sct2+ttl_Sct3+ttl_Sct4, ttl_Sct5);

  for (int i = 0; i < ttl_Sct1+ttl_Sct2; i++) {
    leds[i] = CRGB::Yellow;
  }
  FastLED.show();
}

void loop() {
  int packetSize = Udp.parsePacket();
  
  if (packetSize == buff_sz_1a) {

    Udp.read((char*)pck_Bff_1, buff_sz_1a);
    memcpy(leds, pck_Bff_1, buff_sz_1a);
    
  } else if (packetSize == buff_sz_1b) {

    Udp.read((char*)pck_Bff_1b, buff_sz_1b);
    memcpy(&leds[n_Leds_1a], pck_Bff_1b, buff_sz_1b);
  
  } else if (packetSize == buff_sz_1c) {

    Udp.read((char*)pck_Bff_1c, buff_sz_1c);
    memcpy(&leds[n_Leds_1a+n_Leds_1b], pck_Bff_1c, buff_sz_1c);
  
  } else if (packetSize == buff_sz_2a) {
    Serial.println("2a");
    Udp.read((char*)pck_Bff_2a, buff_sz_2a);
    memcpy(&leds[ttl_Sct1], pck_Bff_2a, buff_sz_2a);
    
  } else if (packetSize == buff_sz_2b) {
    Serial.println("2b");

    Udp.read((char*)pck_Bff_2b, buff_sz_2b);
    memcpy(&leds[ttl_Sct1+n_Leds_2a], pck_Bff_2b, buff_sz_2b);
  
  } else if (packetSize == buff_sz_2c) {
    Serial.println("2c");

    Udp.read((char*)pck_Bff_2c, buff_sz_2c);
    memcpy(&leds[ttl_Sct1+n_Leds_2a+n_Leds_2b], pck_Bff_2c, buff_sz_2c);
  
  } else if (packetSize == 1) {
    FastLED.show();
  }
}
