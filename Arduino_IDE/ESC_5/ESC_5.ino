// https://youtu.be/CZ6PzQZE84U
// for Arduino Uno

#include <Servo.h>

#define MAX_SIGNAL 2000  //PWM信号における最大のパルス幅[マイクロ秒]
#define MIN_SIGNAL 1000  //PWM信号における最小のパルス幅[マイクロ秒]
#define ESC_PIN_1 6  //ESCへの出力ピン
#define ESC_PIN_2 10  //ESCへの出力ピン
int volume = 0;  //ジョイスティックの値を入れる変数
char message[50];  //シリアルモニタへ表示する文字列を入れる変数

Servo esc_1;  //Servoオブジェクトを作成する．今回はESCにPWM信号を送るので，`esc`と命名している．
Servo esc_2;

void setup() {
  Serial.begin(9600);
  Serial.println("Program begin...");
  Serial.println("This program will calibrate the ESC.");
  Serial.println("Turn on power source, then wait 2 seconds and press any key.");
  
  while (!Serial.available());  //シリアルポートで何か入力されるまで待ちます
  Serial.read();

  esc_1.attach(ESC_PIN_1);  //ESCへの出力ピンをアタッチします
  esc_2.attach(ESC_PIN_2);  //ESCへの出力ピンをアタッチします

  Serial.println("Writing minimum output");
  esc_1.writeMicroseconds(MIN_SIGNAL);  //ESCへ最小のパルス幅を指示します
  esc_2.writeMicroseconds(MIN_SIGNAL);  //ESCへ最小のパルス幅を指示します
  Serial.println("Wait 8 seconds. Then motor starts");
  delay(8000);
}

void loop() {  
  volume = analogRead(A0)+1000;  //可変抵抗の値を1.0で掛けて変数volumeに入れる．値を調整したい場合は倍率を変更する．
  sprintf(message, "Pulse Width: %d micro sec", volume);  //シリアルモニタに表示するメッセージを作成
  Serial.println(message);  //可変抵抗の値をシリアルモニタに表示
  esc_1.writeMicroseconds(volume);  // パルス幅 `volume` のPWM信号を送信する
  esc_2.writeMicroseconds(volume);  // パルス幅 `volume` のPWM信号を送信する
}