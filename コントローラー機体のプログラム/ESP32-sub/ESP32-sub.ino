//mainでPS4Controller.hを使用するため、ESP32のバージョンは1.0.6推奨
//CANに関するページ：https://404background.com/circuit/mcp2561-can/
//

#include <Wire.h>
#include <ESP32Servo.h>
#include <CAN.h>
#include "Servo_setup.h"
#include "CAN_receive.h"

ESP32PWM pwm;
char can_receive = ' ';
char pre_can_receive = ' ';

//サブコアでの処理
void SubProcess(void* pvParameters) {
}

void setup() {
  Serial.begin(115200);
  Serial.println("Ready.");

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  pwm.attachPin(27, 20000);  //10khz
  Servo_setup();

  while (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
  }
  //xTaskCreatePinnedToCore(SubProcess, "SubProcess", 4096, NULL, 1, NULL, 0);
}

void loop() {
  //pos3 = -rel_angleY;
  Servo_angle_range();
  Servo_write();
  //Serial.println(rel_angleY);

  int packetSize = CAN.parsePacket();
  CAN_packet();

  // only print packet data for non-RTR packets
  if (CAN.available()) {
    can_receive = (char)CAN.read();
    Serial.println(can_receive);
    switch (can_receive) {
      case '0':
        pos1 = pos1_setup;
        pos2 = pos2_setup;
        pos3 = pos3_setup;
        break;
      case 'f':
        if (pre_can_receive != can_receive) {
          if (servoFlag1 == 0) {
            pos1 = 180;
            servoFlag1 = 1;
          } else {
            pos1 = 0;
            servoFlag1 = 0;
          }
        }
        break;
      case 't':
        if (pre_can_receive != can_receive) {
          if (servoFlag2 == 0) {
            pos2 = 0;
            servoFlag2 = 1;
          } else {
            pos2 = 180;
            servoFlag2 = 0;
          }
        }
        break;

      case 'r':
        pos3 += 1;
        delay(servo_interval);
        break;
      case 'l':
        pos3 += -1;
        delay(servo_interval);
        break;
      case 'p':
        if (pre_can_receive != can_receive) {
          servoFlag = !servoFlag;
          Serial.println(servoFlag);
          break;
        }
        ESP.deepSleep(10000);
      default:
        Serial.print("stop");
    }
    pre_can_receive = can_receive;
  }
  Serial.println();
}
