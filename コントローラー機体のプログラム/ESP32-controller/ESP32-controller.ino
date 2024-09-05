#include <CAN.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "CAN_receive.h"

// #define TFT_RST 16
// #define TFT_DC 17
// #define TFT_CS 5
// Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

int analogStickR = 0;
int analogStickL = 0;
const int pinStickR = 34;
const int pinStickL = 35;
int sensorValue1 = 0;
int sensorValue2 = 0;

const int switchOn = 33;
const int switch1 = 12;
const int switch2 = 13;
const int switch3 = 15;
const int switch4 = 14;
const int switch5 = 27;
const int switch6 = 26;
const int switch7 = 25;
const int switch8 = 32;
const int switch0 = 21;

int joystick1 = 34;
int joystick2 = 35;
int switchPin[] = { switch1, switch2, switch3, switch4, switch5, switch6, switch7, switch8, switch0 };
char canSend[] = { 'j', 'v', 'u', 'd', 'r', 't', 'l', 'f' };
bool flagCAN = 0;
bool flagFor = 0;
char can_send;
char pre_can_send;

void setup() {
  Serial.begin(115200);
  while (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
  }
  // tft.begin();
  for (int i = 0; i < 9; i++) {
    pinMode(switchPin[i], INPUT_PULLUP);
  }
  pinMode(switchOn, INPUT_PULLUP);
}

void loop() {
  sensorValue1 = analogRead(joystick1);
  int vert = map(sensorValue1, 0, 4095, 256, -256);
  sensorValue2 = analogRead(joystick2);
  int horz = map(sensorValue2, 0, 4095, -256, 256);

  if (digitalRead(switchOn) == 1) {
    CAN_packet();
    CAN_receive();
    for (int i = 0; i < 9; i++) {
      while (digitalRead(switchPin[i]) == 0) {
        can_send = canSend[i];
        CAN.beginPacket(0x12);
        CAN.write(can_send);
        CAN.endPacket();
        Serial.print(canSend[i]);
        Serial.print(digitalRead(switchPin[i]));
        flagFor = 1;
        flagCAN = 1;
        delay(10);
      }
      flagFor = 0;
    }
    if (vert > 200) {
      can_send = 'b';
      Serial.print('b');
      CAN.beginPacket(0x12);
      CAN.write(can_send);
      CAN.endPacket();
      flagCAN = 1;
    } else if (200 >= vert && vert >= 50) {
      can_send = 'a';
      Serial.print('a');
      CAN.beginPacket(0x12);
      CAN.write(can_send);
      CAN.endPacket();
      flagCAN = 1;
    } else if (-500 >= vert && vert >= -200) {
      can_send = 'c';
      Serial.print('c');
      CAN.beginPacket(0x12);
      CAN.write(can_send);
      CAN.endPacket();
      flagCAN = 1;
    } else if (vert <= -200) {
      can_send = 'e';
      Serial.print('e');
      CAN.beginPacket(0x12);
      CAN.write(can_send);
      CAN.endPacket();
      flagCAN = 1;
    } else if (horz > 200) {
      can_send = 'h';
      Serial.print('h');
      CAN.beginPacket(0x12);
      CAN.write(can_send);
      CAN.endPacket();
      flagCAN = 1;
    } else if (200 >= horz && horz >= 50) {
      can_send = 'i';
      Serial.print('i');
      CAN.beginPacket(0x12);
      CAN.write(can_send);
      CAN.endPacket();
      flagCAN = 1;
    } else if (-50 >= horz && horz >= -200) {
      can_send = 'k';
      Serial.print('k');
      CAN.beginPacket(0x12);
      CAN.write(can_send);
      CAN.endPacket();
      flagCAN = 1;
    } else if (horz <= -200) {
      can_send = 'm';
      Serial.print('m');
      CAN.beginPacket(0x12);
      CAN.write(can_send);
      CAN.endPacket();
      flagCAN = 1;
    } else {
      if (flagCAN == 1) {
        flagCAN = 0;
        for (int i = 0; i < 3; i++) {
          Serial.println('n');
          CAN.beginPacket(0x12);
          CAN.write('n');
          CAN.endPacket();
        }
      }
    }
    delay(10);
  }
  pre_can_send = can_send;
}
