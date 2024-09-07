// https://youtu.be/CZ6PzQZE84U
// for Arduino Uno

#include <ESP32Servo.h>
ESP32PWM pwm;

#define ESC_PIN_1 26  //ESCへの出力ピン
int volume = 0;       //ジョイスティックの値を入れる変数
char message[50];     //シリアルモニタへ表示する文字列を入れる変数

Servo esc_1;
int servoHz = 50;
int minUs1 = 1000;
int maxUs1 = 2000;

int pre_volume = 0;

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
  pre_volume = analogRead(32);
  Serial.println(pre_volume);
}

void loop() {
  volume = analogRead(32) - pre_volume + 500;             //可変抵抗の値を1.0で掛けて変数volumeに入れる．値を調整したい場合は倍率を変更する．
  sprintf(message, "Pulse Width: %d micro sec", volume);  //シリアルモニタに表示するメッセージを作成
  Serial.println(message);                                //可変抵抗の値をシリアルモニタに表示
  esc_1.writeMicroseconds(volume);         // パルス幅 `volume` のPWM信号を送信する
}