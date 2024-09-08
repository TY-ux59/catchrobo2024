//PS4Controller.hを使用するため、ESP32のバージョンは1.0.6推奨
//PS4Controller.hに関するページ（環境構築）：https://404background.com/program/esp32-dualshock4/
//PS4Controller.hに関するページ（修正後ライブラリ）：https://404background.com/program/esp32-dualshock4-library/
//https://github.com/sandeepmistry/arduino-CAN
//https://github.com/404background/PS4-esp32

#include <PS4Controller.h>
#include <CAN.h>
#include "RotalyEncoder.h"
#include "MotorDriver.h"
#include "Auto.h"
#include "CAN_receive.h"

int angleA = 0;
int move2 = 0;

int preMillis = 0;
bool LStickFlag = false;
int LStickX = 0;
int preLStickX = 0;
char can_receive = ' ';
char pre_can_receive = ' ';
char pre_can_send = ' ';
bool handOnce = 0;
bool constState = false;

void setup() {
  Motor_stop();
  Serial.begin(115200);
  PS4.begin();  //write PS4 id Macアドレスを書き込む
  Serial.println("Ready.");

  MotorDriver_setup();
  RotalyEncoderSetup();
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
  }
  preMillis = millis();
}
void loop() {
  handOnce = 0;

  /*Serial.print(" R1: ");
  Serial.println(rotation1, DEC);
  Serial.print(" R2: ");
  Serial.println(rotation2, DEC);
  Serial.print(" R3: ");
  Serial.println(rotation3, DEC);
  Serial.print(" angleA: ");
  Serial.println(angleA, DEC);*/
  CAN_packet();
  can_receive = CAN_receive();

  switch (can_receive) {
    //up
    case 'j':
      digitalWrite(PWM2, HIGH);
      digitalWrite(DIR2, HIGH);
      break;
    //down
    case 'v':
      digitalWrite(PWM2, HIGH);
      digitalWrite(DIR2, LOW);
      break;
    //棒を前へ
    case 'u':
      digitalWrite(PWM3, HIGH);
      digitalWrite(DIR3, HIGH);
    //棒を後ろへ
    case 'd':
      digitalWrite(PWM3, HIGH);
      digitalWrite(DIR3, LOW);
      break;
    //機体を前後
    case 'b':
      ledcWrite(0, 200);
      digitalWrite(DIR4, HIGH);
      break;
    //機体を前後
    case 'a':
      ledcWrite(0, 100);
      digitalWrite(DIR4, HIGH);
      break;
    //機体を前後
    case 'c':
      ledcWrite(0, 100);
      digitalWrite(DIR4, LOW);
      break;
    //機体を前後
    case 'e':
      ledcWrite(0, 200);
      digitalWrite(DIR4, LOW);
      break;
    //回転
    case 'h':
      ledcWrite(1, 200);
      digitalWrite(DIR1, LOW);
      break;
    //回転
    case 'i':
      ledcWrite(1, 100);
      digitalWrite(DIR1, LOW);
      break;
    //回転
    case 'k':
      ledcWrite(1, 100);
      digitalWrite(DIR1, HIGH);
      break;
    //回転
    case 'm':
      ledcWrite(1, 200);
      digitalWrite(DIR1, HIGH);
      break;
    default:
      Motor_stop();
  }
  Serial.print(can_receive);
  PS4_control();
}


void PS4_control() {
  //PS4のコントローラ用
  //機体を取る部分を上げ下げする
  if (PS4.Up()) {  //motor2 forward.
    //Serial.println("L1 Button");
    if (can_receive == 'y') {
      digitalWrite(PWM2, LOW);
    } else {
      digitalWrite(PWM2, HIGH);
    }
    digitalWrite(DIR2, LOW);
  } else if (PS4.Down()) {  //motor2 backward.
    //Serial.println("L2 Button");
    if (can_receive == 'y') {
      digitalWrite(PWM2, LOW);
    } else {
      digitalWrite(PWM2, HIGH);
    }
    digitalWrite(DIR2, HIGH);
    //棒を前後
  } else if (PS4.L2()) {  //motor3 up.
    //Serial.println("R1 Button");
    if (can_receive == 'z') {
      digitalWrite(PWM3, LOW);
    } else {
      digitalWrite(PWM3, HIGH);
    }
    digitalWrite(DIR3, LOW);
  } else if (PS4.R2()) {  //motor down.
    //Serial.println("R2 Button");
    if (can_receive == 'z') {
      digitalWrite(PWM3, LOW);
    } else {
      digitalWrite(PWM3, HIGH);
    }
    digitalWrite(DIR3, HIGH);

  } else if (PS4.Cross()) {
    Serial.println("CrossButton");
    CAN.beginPacket(0x12);
    CAN.write('m');
    delay(100);
    CAN.endPacket();
  } else if (PS4.Circle()) {
    //vacuumBoardフォルダにcan通信
    Serial.println("Circle");
    CAN.beginPacket(0x12);
    CAN.write('0');
    delay(100);
    CAN.endPacket();

    //Ebiを取るときに開けっぱなしにする
    //仕分け部分のモーターと吸う部分のモーターを開けっぱなしにする
    //constStateは開けっ放しかどうかtrueであきっぱなし
    if (constState == false) {
      Serial.println("constOpen");
      constState = true;
    } else {
      Serial.println("ret");
      constState = false;
    }

  } else if (PS4.Square()) {
    //vacuumBoardフォルダにcan通信
    Serial.println("Square");
    CAN.beginPacket(0x12);
    CAN.write('3');
    delay(100);
    CAN.endPacket();

    //Yuzuを取るときに開けっぱなしにする
    //仕分け部分のモーターと吸う部分のモーターを開けっぱなしにする
    if (constState == false) {
      Serial.println("constOpen");
      constState = true;
    } else {
      Serial.println("ret");
      constState = false;
    }
  } else if (PS4.Triangle()) {
    //vacuumBoardフォルダにcan通信
    Serial.println("Triangle");
    CAN.beginPacket(0x12);
    CAN.write('4');
    delay(100);
    CAN.endPacket();

    //Noriを取るときに開けっぱなしにする
    //仕分け部分のモーターと吸う部分のモーターを開けっぱなしにする
    if (constState == false) {
      Serial.println("constOpen");
      constState = true;
    } else {
      Serial.println("ret");
      constState = false;
    }
  } else {
    Serial.println('n');
    CAN.beginPacket(0x12);
    CAN.write('n');
    CAN.endPacket();
  }

  //ラズベリーパイとのシリアル通信
  if (Serial.available() > 0) {
    String receivedMessage = Serial.readStringUntil('\n');

    if (receivedMessage == "M2close") {
      CAN.beginPacket(0x12);
      CAN.write('5');
      delay(100);
      CAN.endPacket();
    }
    if (receivedMessage == "M2Open") {
      CAN.beginPacket(0x12);
      CAN.write('6');
      delay(100);
      CAN.endPacket();
    }

    if (receivedMessage == "EbiM1") {
      CAN.beginPacket(0x12);
      CAN.write('7');
      delay(100);
      CAN.endPacket();
    } else if (receivedMessage == "YuzuM1") {
      CAN.beginPacket(0x12);
      CAN.write('8');
      delay(100);
      CAN.endPacket();
    } else if (receivedMessage == "NoriM1") {
      CAN.beginPacket(0x12);
      CAN.write('9');
      delay(100);
      CAN.endPacket();
    }
  }

  //Serial.printf("Right Stick y at %d\n", PS4.RStickY());
  //スティックの傾きに合わせて、回転速度を調整
  //Lスティックの反応が悪かったため、Rスティックに変更
  int RStickY = map(PS4.RStickY(), -128, 128, -256, 256);
  if (RStickY <= 20 && RStickY >= -20) {
    ledcWrite(0, 0);
  } else {
    if (can_receive == 'x') {
      digitalWrite(PWM4, LOW);
    } else {
      ledcWrite(0, abs(RStickY));
    }
  }
  //回転方向
  if (RStickY >= 0) {  //left
    digitalWrite(DIR4, HIGH);
  } else {  ///right
    digitalWrite(DIR4, LOW);
  }

  LStickX = map(PS4.LStickX(), -128, 128, -256, 256);
  if (LStickX <= 20 && LStickX >= -20) {
    if (LStickFlag == true) {
      int nowMillis = millis();
      if (nowMillis - preMillis >= 100) {
        if (preLStickX >= 20) {
          preLStickX -= 10;
          ledcWrite(1, abs(preLStickX));
        } else if (LStickX <= -20) {
          preLStickX += 10;
          ledcWrite(1, abs(preLStickX));
        } else {
          preLStickX = 0;
          LStickFlag = false;
          ledcWrite(1, 0);
        }
        preMillis = nowMillis;
        Serial.println(LStickX);
      }
    }
  } else {
    ledcWrite(1, abs(LStickX));
    LStickFlag = true;
    preLStickX = LStickX;
  }
  //回転方向
  if (LStickX >= 0) {  //back
    digitalWrite(DIR1, LOW);
  } else {  //forward
    digitalWrite(DIR1, HIGH);
  }
}
