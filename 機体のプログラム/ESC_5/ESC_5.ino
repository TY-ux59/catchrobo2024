// https://youtu.be/CZ6PzQZE84U
// for Arduino Uno

#include <PS4Controller.h>
#include <CAN.h>
#include <ESP32Servo.h>
#include "CAN_receive.h"
ESP32PWM pwm;

#define ESC_PIN_1 18  //ESCへの出力ピン

char message[50];  //シリアルモニタへ表示する文字列を入れる変数

Servo esc_1;
int servoHz = 50;
int minUs1 = 1000;
int maxUs1 = 2000;

int volume = minUs1;

char can_receive = ' ';

//サブコアでの処理
void SubProcess(void* pvParameters) {
}

void setup() {
  Serial.begin(115200);
  pinMode(32, INPUT);

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  pwm.attachPin(27, 20000);  //10khz

  esc_1.setPeriodHertz(servoHz);            // Standard 50hz servo
  esc_1.attach(ESC_PIN_1, minUs1, maxUs1);  //ESCへの出力ピンをアタッチします

  Serial.println("Writing minimum output");
  esc_1.writeMicroseconds(minUs1);  //ESCへ最小のパルス幅を指示します

  Serial.println("Wait 8 seconds. Then motor starts");
  delay(8000);

  while (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
  }
}

void loop() {
  CAN.parsePacket();
  CAN_packet();
  // そうじ機のモーターを1200から1300ぐらいにあげていきたい
  if (CAN.available()) {
    can_receive = (char)CAN.read();
    Serial.println(can_receive);
    if (can_receive == 't') {
      while (volume < 1300) {
        volume += 2;
        delay(100);
        sprintf(message, "Pulse Width: %d micro sec", volume);  //シリアルモニタに表示するメッセージを作成
        Serial.println(message);                                //可変抵抗の値をシリアルモニタに表示
        esc_1.writeMicroseconds(volume);                        // パルス幅 `volume` のPWM信号を送信する
      }
    } else if (can_receive == 'n') {
      volume = 1000;
      esc_1.writeMicroseconds(volume);
    }
  }
}
