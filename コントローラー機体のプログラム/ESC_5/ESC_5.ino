// https://youtu.be/CZ6PzQZE84U
// for Arduino Uno

#include <ESP32Servo.h>
#include <PS4Controller.h>
#define MAX_SIGNAL 2000  //PWM信号における最大のパルス幅[マイクロ秒]
#define MIN_SIGNAL 1000  //PWM信号における最小のパルス幅[マイクロ秒]
#define ESC_PIN_1 18     //ESCへの出力ピン
int volume = 0;          //ジョイスティックの値を入れる変数
char message[50];        //シリアルモニタへ表示する文字列を入れる変数

Servo esc_1;  //Servoオブジェクトを作成する．今回はESCにPWM信号を送るので，`esc`と命名している．
Servo esc_2;

int pre_volume = 0;

void setup() {
  Serial.begin(115200);
  pinMode(32, INPUT);
  pre_volume = analogRead(32);

  esc_1.attach(ESC_PIN_1);  //ESCへの出力ピンをアタッチします

  Serial.println("Writing minimum output");
  esc_1.writeMicroseconds(MIN_SIGNAL);  //ESCへ最小のパルス幅を指示します

  Serial.println("Wait 8 seconds. Then motor starts");
  delay(8000);
}

void loop() {
  if (PS4.Cross()) {
    //そうじ機で吸う
    volume = analogRead(32) - pre_volume + 2000;            //可変抵抗の値を1.0で掛けて変数volumeに入れる．値を調整したい場合は倍率を変更する．
    sprintf(message, "Pulse Width: %d micro sec", volume);  //シリアルモニタに表示するメッセージを作成
    Serial.println(message);                                //可変抵抗の値をシリアルモニタに表示
    esc_1.writeMicroseconds(volume);                        // パルス幅 `volume` のPWM信号を送信する
  }
}
