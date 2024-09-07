// https://youtu.be/CZ6PzQZE84U
// for Arduino Uno

#include <PS4Controller.h>
#include <CAN.h>
#include <ESP32Servo.h>
#include "CAN_receive.h"
#include <stdio.h>

ESP32PWM pwm;

#define ESC_PIN_1 18  //ESCへの出力ピン

char message[50];  //シリアルモニタへ表示する文字列を入れる変数

Servo esc_1;
Servo servo2;
Servo servo3;
Servo servo4;

int servoHz = 50;
int minUs1 = 1000;
int maxUs1 = 2000;

int volume = minUs1;

char can_receive = ' ';

bool constState = false;



//サブコアでの処理
void SubProcess(void* pvParameters) {
}

void setup() {
  Serial.begin(115200);
  pinMode(32, INPUT);
  //サーボモーターの制御信号ピンを指定
  servo2.attach(25);
  servo3.attach(26);
  servo4.attach(27);
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

  servo3.write(45);

  while (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
  }
}

void loop() {
  //can通信
  CAN.parsePacket();
  // そうじ機のモーターを1200から1300ぐらいにあげていきたい
  if (CAN.available()) {

    CAN_packet();
    can_receive = CAN_receive();
    Serial.println(can_receive);
    if (can_receive == 'm') {
      while (volume < 1300) {
        volume += 50;
        delay(100);
        sprintf(message, "Pulse Width: %d micro sec", volume);  //シリアルモニタに表示するメッセージを作成
        Serial.println(message);                                //可変抵抗の値をシリアルモニタに表示
        esc_1.writeMicroseconds(volume);                        // パルス幅 `volume` のPWM信号を送信する
      }
    } else if (constState == true && ((can_receive == '0') || (can_receive == '3') || (can_receive == '4'))) {
      servo3.write(0);
      delay(250);
      constState = false;
    } else if (can_receive == 'n') {
      volume = 1000;
      esc_1.writeMicroseconds(volume);
    }

    //開いている状態を維持するかどうか
    if (can_receive == '0' && constState == false) {
      servo2.write(45);
      delay(250);
      constState = true;
      servo3.write(30);
      delay(250);
    } else if (can_receive == '3' && constState == false) {
      servo2.write(45);
      delay(250);
      constState = true;
      servo3.write(60);
      delay(250);
    } else if (can_receive == '4' && constState == false) {
      servo2.write(45);
      delay(250);
      constState = true;
      servo3.write(90);
      delay(250);
    }

    //ラズベリーパイによる開け閉めの判断
    //そうじ機についているモーター
    if (can_receive == '5') {
      servo2.write(0);
    } else if (can_receive == '6') {
      servo2.write(45);
    }

    if (can_receive == '7') {
      servo3.write(30);
      delay(500);
      servo3.write(0);
    } else if (can_receive == '8') {
      servo3.write(60);
      delay(500);
      servo3.write(0);
    } else if (can_receive == '9') {
      servo3.write(90);
      delay(500);
      servo3.write(0);
    }
  }
}
