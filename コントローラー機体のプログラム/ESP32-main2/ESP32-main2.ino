#include <CAN.h>
#include "HC-SR04.h"
#include "Switch.h"
#include "CAN_receive.h"

char can_receive = ' ';
char pre_can_receive = ' ';

void setup() {
  Serial.begin(115200);
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
  }
  HCSR04_setup();
  SwitchSetup();
}

void loop() {
  dist1 = read_distance(TRIG1, ECHO1);
  dist2 = read_distance(TRIG2, ECHO2);
  Serial.print(dist1);
  Serial.print("   ");
  Serial.print(dist2);
  Serial.print(SwitchStates(switch1));
  Serial.print(SwitchStates(switch2));
  Serial.print(SwitchStates(switch3));
  Serial.println();


  CAN_packet();


  if (SwitchStates(switch1) == 0) {
    CAN.beginPacket(0x12);
    CAN.write('x');
    CAN.endPacket();
    Serial.println("x");
  }
  if (SwitchStates(switch2) == 0) {
    CAN.beginPacket(0x12);
    CAN.write('y');
    CAN.endPacket();
    Serial.println("y");
  }
  if (SwitchStates(switch3) == 0) {
    CAN.beginPacket(0x12);
    CAN.write('z');
    CAN.endPacket();
    Serial.println("z");
  }

  can_receive = CAN_receive();
  Serial.println(can_receive);

  switch (can_receive) {
    case '0':
      if (dist1 < 50) {
        CAN.beginPacket(0x12);
        CAN.write('3');
        CAN.endPacket();
      }
      //if (dist1 < 50) {
      CAN.beginPacket(0x12);
      CAN.write('4');
      CAN.endPacket();
      break;
  }
}
