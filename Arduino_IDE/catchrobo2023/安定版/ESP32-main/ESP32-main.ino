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
  //ロータリーエンコーダ用
  valA = JudgeHighLow(digitalRead(pin1_encoder1));
  valB = JudgeHighLow(digitalRead(pin2_encoder1));
  valC = JudgeHighLow(digitalRead(pin1_encoder2));
  valD = JudgeHighLow(digitalRead(pin2_encoder2));
  valE = JudgeHighLow(digitalRead(pin1_encoder3));
  valF = JudgeHighLow(digitalRead(pin2_encoder3));
  valG = JudgeHighLow(digitalRead(pin1_encoder4));
  valH = JudgeHighLow(digitalRead(pin2_encoder4));
  rotation1 = rotalyEncoder(valA, valB, preValA, preValB, pin1_encoder1, pin2_encoder1, rotation1);  //ロータリーエンコーダー３つの挙動
  rotation2 = rotalyEncoder(valC, valD, preValC, preValD, pin1_encoder2, pin2_encoder2, rotation2);
  rotation3 = rotalyEncoder(valE, valF, preValE, preValF, pin1_encoder3, pin2_encoder3, rotation3);
  rotation4 = rotalyEncoder(valG, valH, preValG, preValH, pin1_encoder4, pin2_encoder4, rotation4);
  preValA = valA;
  preValB = valB;
  preValC = valC;
  preValD = valD;
  preValE = valE;
  preValF = valF;
  preValG = valG;
  preValH = valH;
  Serial.print(" R1: ");
  Serial.println(rotation1, DEC);
  Serial.print(" R2: ");
  Serial.println(rotation2, DEC);
  Serial.print(" R3: ");
  Serial.println(rotation3, DEC);
  Serial.print(" angleA: ");
  Serial.println(angleA, DEC);
  CAN_packet();
  can_receive = CAN_receive();

  switch (can_receive) {
    case 'j':
      digitalWrite(PWM2, HIGH);
      digitalWrite(DIR2, HIGH);
      break;
    case 'v':
      digitalWrite(PWM2, HIGH);
      digitalWrite(DIR2, LOW);
      break;
    case 'u':
      digitalWrite(PWM3, HIGH);
      digitalWrite(DIR3, HIGH);
    case 'd':
      digitalWrite(PWM3, HIGH);
      digitalWrite(DIR3, LOW);
      break;
    case 'b':
      ledcWrite(0, 200);
      digitalWrite(DIR4, HIGH);
      break;
    case 'a':
      ledcWrite(0, 100);
      digitalWrite(DIR4, HIGH);
      break;
    case 'c':
      ledcWrite(0, 100);
      digitalWrite(DIR4, LOW);
      break;
    case 'e':
      ledcWrite(0, 200);
      digitalWrite(DIR4, LOW);
      break;
    case 'h':
      ledcWrite(1, 200);
      digitalWrite(DIR1, LOW);
      break;
    case 'i':
      ledcWrite(1, 100);
      digitalWrite(DIR1, LOW);
      break;
    case 'k':
      ledcWrite(1, 100);
      digitalWrite(DIR1, HIGH);
      break;
    case 'm':
      ledcWrite(1, 200);
      digitalWrite(DIR1, HIGH);
      break;
    default:
      Motor_stop();
  }
  Serial.print(can_receive);

  PS4_control();

  result1 = pid(rotation1, goal1);  //"rotation"を現在地、"goal"を目的地としてPID制御を行う
  result2 = pid(rotation2, goal2);
  result3 = pid(rotation3, goal3);
  result4 = pid(rotation4, goal4);
  /*Serial.print(" Result1: ");
    Serial.print(result1, DEC);
    Serial.print(" Result2: ");
    Serial.print(result2, DEC);
    Serial.print(" Result3: ");
    Serial.print(result3, DEC);*/
  Serial.println();
  //Serial.println(LStickX, DEC);
  motorEnd(valA, valB, preValA, preValB, difRotation1, preRotation1, rotation1);
  motorEnd(valC, valD, preValC, preValD, difRotation2, preRotation2, rotation2);
  motorEnd(valE, valF, preValE, preValF, difRotation3, preRotation3, rotation3);
}


void PS4_control() {
  //PS4のコントローラ用
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
  } else if (PS4.Touchpad()) {
    rotation3 = 0;
    int i = 0;
    while (i <= 40) {
      if (PS4.PSButton()) break;
      valE = JudgeHighLow(digitalRead(pin1_encoder3));
      valF = JudgeHighLow(digitalRead(pin2_encoder3));
      rotation3 = rotalyEncoder(valE, valF, preValE, preValF, pin1_encoder3, pin2_encoder3, rotation3);
      preValE = valE;
      preValF = valF;
      ledcWrite(0, pid(rotation3, goal3));
      if (goal3 - rotation3 >= 0 && rotation3 >= -3) {
        digitalWrite(DIR4, LOW);
      } else {
        digitalWrite(DIR4, HIGH);
      }
      if (abs(rotation3 - goal3) <= 10) {
        i++;
      } else i = 0;
    }
    i = 0;
    ledcWrite(0, 0);
    sumDif = 0;
    preDif = 0;
  } else if (PS4.Circle()) {
    rotation1 = 0;
    angleA = 0;
    int i = 0;
    while (i <= 40) {
      if (PS4.PSButton()) break;
      valA = JudgeHighLow(digitalRead(pin1_encoder1));
      valB = JudgeHighLow(digitalRead(pin2_encoder1));
      rotation1 = rotalyEncoder(valA, valB, preValA, preValB, pin1_encoder1, pin2_encoder1, rotation1);
      preValA = valA;
      preValB = valB;
      rotation1 = constrain(rotation1, -320, 320);
      angleA = map(rotation1, -320, 320, -180, 180);
      ledcWrite(1, pid(angleA, goal1));
      Serial.println(angleA);
      Serial.println(i);
      // if (goal1 - angleA >= 0 && angleA >= -3) {
      digitalWrite(DIR1, LOW);
      /*} else {
        digitalWrite(DIR1, HIGH);
      }*/
      if (abs(angleA - goal1) <= 7) {
        i++;
      } else i = 0;
    }
    i = 0;
    ledcWrite(1, 0);
    sumDif = 0;
    preDif = 0;
  } else if (PS4.Square()) {
    rotation1 = 0;
    angleA = 0;
    int i = 0;
    while (i <= 40) {
      if (PS4.PSButton()) break;
      valA = JudgeHighLow(digitalRead(pin1_encoder1));
      valB = JudgeHighLow(digitalRead(pin2_encoder1));
      rotation1 = rotalyEncoder(valA, valB, preValA, preValB, pin1_encoder1, pin2_encoder1, rotation1);
      preValA = valA;
      preValB = valB;
      rotation1 = constrain(rotation1, -320, 320);
      angleA = map(rotation1, -320, 320, -180, 180);
      ledcWrite(1, pid((-1 * angleA), goal1));
      Serial.println(angleA);
      Serial.println(i);
      //if (goal1 + angleA >= 0 && angleA <= 3) {
      digitalWrite(DIR1, HIGH);
      /*} else {
        digitalWrite(DIR1, LOW);
      }*/
      if (abs((-1 * angleA) - goal1) <= 7) {
        i++;
      } else i = 0;
    }
    i = 0;
    ledcWrite(1, 0);
    sumDif = 0;
    preDif = 0;
  }
  /*else if(PS4.Triangle()) {
    rotation2 = 0;
    int i = 0;
    while (i <= 40) {
      valC = JudgeHighLow(digitalRead(pin1_encoder2));
      valD = JudgeHighLow(digitalRead(pin2_encoder2));
      rotation2 = rotalyEncoder(valC, valD, preValC, preValD, pin1_encoder2, pin2_encoder2, rotation2);
      preValC = valC;
      preValD = valD;
      digitalWrite(PWM2, HIGH);
      if (goal2 - rotation2 >= 0 && rotation2 >= -3) {
        digitalWrite(DIR2, LOW);
      } else {
        digitalWrite(DIR2, HIGH);
      }
      if (abs(rotation2 - goal2) <= 10) {
        i++;
      }
      else i = 0;
    }
    sumDif = 0;
    preDif = 0;
  }
  else if(PS4.Cross()) {
    rotation2 = 0;
     int i = 0;
    while (i <= 40) {
      valG = JudgeHighLow(digitalRead(pin1_encoder3));
      valH = JudgeHighLow(digitalRead(pin2_encoder3));
      rotation4 = rotalyEncoder(valG, valH, preValG, preValH, pin1_encoder4, pin2_encoder4, rotation4);
      preValG = valG;
      preValH = valH;
      digitalWrite(PWM3, HIGH);
      if (goal4 - rotation4 >= 0 && rotation4 >= -3) {
        digitalWrite(DIR3, LOW);
      } else {
        digitalWrite(DIR3, HIGH);
      }
      if (abs(rotation4 - goal4) <= 10) {
        i++;
      }
      else i = 0;
    }
    sumDif = 0;
    preDif = 0;
  }*/
  //CAN通信用
  else if (PS4.Options()) {
    CAN.beginPacket(0x12);
    CAN.write('p');
    CAN.endPacket();
  }
  /*
    else if (PS4.Square()) {
    //Serial.println("Square Button");
    CAN.beginPacket(0x12);
    CAN.write('s');
    CAN.endPacket();
    } else if (PS4.Triangle()) {
    //Serial.println("Triangle Button");
    CAN.beginPacket(0x12);
    CAN.write('g');
    CAN.endPacket();
    } else if (PS4.Cross()) {
    //Serial.println("Cross Button");
    CAN.beginPacket(0x12);
    CAN.write('c');
    CAN.endPacket();
    } else if (PS4.Circle()) {
    //Serial.println("Circle Button");
    CAN.beginPacket(0x12);
    CAN.write('o');
    CAN.endPacket();
    }
  */
  else if (PS4.Right()) {
    //Serial.println("Right");
    CAN.beginPacket(0x12);
    CAN.write('l');
    CAN.endPacket();
  } else if (PS4.Left()) {
    //Serial.println("Left");
    CAN.beginPacket(0x12);
    CAN.write('r');
    CAN.endPacket();
  } else if (PS4.L1()) {
    if (handOnce == 0) {
      CAN.beginPacket(0x12);
      CAN.write('t');
      CAN.endPacket();
      handOnce++;
    }
  } else if (PS4.R1()) {
    if (handOnce == 0) {
      CAN.beginPacket(0x12);
      CAN.write('f');
      CAN.endPacket();
      handOnce++;
    }
  } else {
    CAN.beginPacket(0x12);
    CAN.write('n');
    CAN.endPacket();
    digitalWrite(PWM1, LOW);
    digitalWrite(PWM2, LOW);
    digitalWrite(PWM3, LOW);
    digitalWrite(PWM4, LOW);
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

  //Serial.printf("Left Stick x at %d\n", PS4.LStickX());
  //スティックの傾きに合わせて、回転速度を調整
  /*
    if (LStickX <= 20 && LStickX >= -20) {
    ledcWrite(1, 0);
    LStickFlag=true;
    } else {
    ledcWrite(1, abs(LStickX));
    }
    int nowMillis = millis();
    if (LStickFlag == true) {
    if (nowMillis - preMillis >= 1000) {
      if (LStickX >= 20) {
        LStickX -= 10;
        ledcWrite(1, abs(LStickX));
      }
      else if (LStickX <= -20) {
        LStickX += 10;
        ledcWrite(1, abs(LStickX));
      }
      else {
        LStickX = 0;
        LStickFlag = false;
      }
      preMillis = nowMillis;
    }
    }
    else LStickX = map(PS4.LStickX(), -128, 128, -256, 256);
  */
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

//初期位置設定用
void AutoSetup() {
  int endFlag = 0;
  int motorState1 = 50;
  bool motorState2 = HIGH;
  bool motorState3 = HIGH;
  int motorState4 = 50;
  digitalWrite(DIR4, HIGH);
  while (endFlag >= 1) {
    CAN_packet();
    char can_receive = CAN_receive();
    /*
      ledcWrite(1, motorState1);
      digitalWrite(PWM2, motorState2);
      digitalWrite(PWM3, motorState3);
    */


    ledcWrite(0, motorState4);

    switch (can_receive) {
      /*
        case 'y':
        motorState3 = LOW;
        endFlag += 1;
        break;
      */
      case 'x':
        motorState4 = 0;
        endFlag += 1;
        break;
        /*
          case 'z':
          motorState1 = 0;
          endFlag += 1;
          break;
          case '4':
          motorState2 = LOW;
          endFlag += 1;
          break;
        */
    }
  }
  Motor_stop();
}
